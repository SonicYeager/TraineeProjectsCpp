#include "ExportEngine.h"
#include "VideoEngine.h"
#include <iostream>
#include <chrono>
#include "ArrangmentData.h"

#include "ColorSpaceConverter.h"
#include "WrappedVideoEngine.h"
#include "FeatureProtection.h"
#include "FilesystemHandler.h"
#include "ProgressHandler.h"
#include "Measurement.h"
#include "LogHandler.h"
#include "ExportHandler.h"

bool CanExportNextFrame()
{
	return true;
}

bool ExportEngine::Bounce(const ExportEngineConfig& config)
{
	int result{};
	try
	{
		result = -1;
		ExportHandler expHandler{config.pExporter, config.createExport, static_cast<ExportFlags>(config.flagsExport)};
		if(CheckBounceIsValid(expHandler.GetExportConfig(), config))
		{
			ThrowIfProtectedFeature(expHandler.GetExportConfig());
			Progress progress{config.pUserInterface};
			progress.OpenProgress(config.pPI->rangeEnd - config.pPI->rangeStart);
			auto targetPath = ConfigDirectory(static_cast<ExportFlags>(config.flagsExport) == ExportFlags::RENAME_FILENAME_IF_EXIST, config.targetFileName);
			WrappedVideoEngine wVideoEng;
			wVideoEng.Prepare(*config.pPI);
			expHandler.Initialize(targetPath);
			LogExportRange(config.pPI->rangeStart, config.pPI->rangeEnd, targetPath.string());
			Measurement measurement;
			measurement.Start();
			for(__int64 i = config.pPI->rangeStart; i < config.pPI->rangeEnd; i += static_cast<__int64>(config.pPI->frameRate))
			{
				if(progress.IsAborded(result))
				{
					std::cout << "aborted by user";
					break;
				}

				auto exConfig = expHandler.GetExportConfig();
				auto videoframe = wVideoEng.GetNextFrame();
				ConvertToYUV(videoframe, exConfig.format);
				auto written = expHandler.ExportVideoFrame(std::move(videoframe));
				progress.AddProgress(written);
			}
			measurement.Stop();
			LogExportTime(config.pPI->rangeEnd - config.pPI->rangeStart / config.pPI->frameRate, measurement.GetElapsedTime());
			result = 1;
		}
	}
	catch(std::exception ex)
	{
		std::cerr << "error during export occurred: " << ex.what();
	}
	return result == 1;
}

bool ExportEngine::CheckBounceIsValid(const ExportConfig& exConfig, const ExportEngineConfig& config) const
{
	if(config.flagsExport & BOUNCE_IF_VALID and config.pPI)
	{
		return config.pPI->aspectRation == exConfig.aspectRatio and config.pPI->width >= exConfig.width and config.pPI->height >= exConfig.height and not config.targetFileName.empty();
	}
	return false;
}
