#include "ExportEngine.h"
#include "VideoEngine.h"
#include <iostream>
#include <chrono>
#include "ArrangmentData.h"
#include "ColorSpaceConverter.h"
#include "FilesystemHandler.h"
#include "ProgressHandler.h"
#include "Measurement.h"
#include "Log.h"

bool ExportEngine::Bounce(const ExportEngineConfig& config)
{
	VideoEngine vidEngine{};
	int result;
	try
	{
		m_config = config;
		result = -1;
		m_pExporter = IVideoExport::ConfigExporter(m_config);
		if(CheckBounceIsValid())
		{
			Log log{};
			CheckFeatureProtection(m_pExporter);
			FilesystemHandler fsHandler{};
			fsHandler.FindOtherFile(m_config);
			fsHandler.ConfigPath(m_config, log);
			ProgressHandler prgHandler{m_config.pUserInterface};
			prgHandler.OpenProgress(m_config);
			vidEngine.PrepareVideoEngine(*m_config.pPI);
			m_pExporter->Initialize(m_config.targetFileName);
			log.LogFileName(m_config);
			log.LogRange(m_config);
			Measurement measure{};
			measure.Start();
			size_t totalWritten = 0;
			for(__int64 i{m_config.pPI->rangeStart}; i < m_config.pPI->rangeEnd; i += static_cast<__int64>(m_config.pPI->frameRate))
			{
				prgHandler.ThrowIFProgressAbort(result);
				auto videoframe = vidEngine.GetFrame(i);
				ColorSpaceConverter csc{};
				csc.ConvertFrameColorFormat(m_pExporter, videoframe);
				m_pExporter->WriteFrame(videoframe, totalWritten, prgHandler);
			}
			measure.Stop();
			log.LogExport(measure, m_config);
			result = 1;
		}
	}
	catch(std::exception ex)
	{
		std::cerr << "error during export occurred: " << ex.what();
	}
	catch(int)
	{
		std::cout << "aborted by user";
	}
	vidEngine.ShutdownVideoEngine();
	m_pExporter = nullptr;
	m_config = {};
	return result == 1;
}

bool ExportEngine::CheckBounceIsValid() const
{
	if(m_config.flagsExport & BOUNCE_IF_VALID and m_pExporter and m_config.pPI)
	{
		ExportConfig exConfig{};
		m_pExporter->GetExportInfo(&exConfig);
		return m_config.pPI->aspectRation == exConfig.aspectRatio
			and m_config.pPI->width >= exConfig.width
			and m_config.pPI->height >= exConfig.height
			and not m_config.targetFileName.empty();
	}
	return false;
}

void ExportEngine::CheckFeatureProtection(IVideoExport* pExporter) const
{
	if(pExporter)
	{
		ExportConfig config{};
		pExporter->GetExportInfo(&config);
		if(not(config.type == ExportType::DVD or config.type == ExportType::MP4))
			throw std::exception("Feature not allowed");
	}
}
