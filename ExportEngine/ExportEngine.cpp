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


IVideoExport* ConfigExporter(const ExportEngineConfig& exporterConfig)
{
	if(exporterConfig.pExporter)
	{
		return exporterConfig.pExporter;
	}
	else
	{
		if(exporterConfig.createExport)
			return exporterConfig.createExport(exporterConfig.flagsExport & RGB_EXPORT ? ExportColorFormat::RGB : ExportColorFormat::YUV);
		else
			throw std::exception("no export available");
	}
}

void LogRange(const ExportEngineConfig& config)
{
	if(config.pPI->rangeEnd > config.pPI->rangeStart)
		std::cout << " from " << config.pPI->rangeStart << " to " << config.pPI->rangeEnd << " started.\n";
}

void LogExport(const Measurement& measure, const ExportEngineConfig& config)
{
	const double expLen = config.pPI->rangeEnd - config.pPI->rangeStart / config.pPI->frameRate;
	std::cout << "Export " << std::fixed << std::setprecision(1) << expLen
			  << "s finished successful (Duration=" << measure.GetPassesTime() << " ms)\n";
}

bool ExportEngine::Bounce(const ExportEngineConfig& config)
{
	VideoEngine vidEngine{};
	try
	{
		size_t totalWritten = 0;
		m_config = config;
		m_Result = -1;
		m_pExporter = ConfigExporter(m_config);
		if(CheckBounceIsValid())
		{
			CheckFeatureProtection(m_config.pExporter);
			FilesystemHandler fsHandler{};
			fsHandler.FindOtherFile(m_config);
			fsHandler.ConfigPath(m_config);
			ProgressHandler prgHandler{m_config.pUserInterface};
			prgHandler.OpenProgress(m_config);
			vidEngine.PrepareVideoEngine(*m_config.pPI);
			m_pExporter->Initialize(m_config.targetFileName);
			Log log{};
			log.LogFileName(m_config);
			LogRange(m_config);
			Measurement measure{};
			measure.Start();
			//{ VALIDATEFRAMES-this | CONVERTFRAMES-Converter | WRITEFRAMES-this
			for(__int64 i = m_config.pPI->rangeStart; i < m_config.pPI->rangeEnd;)
			{
				prgHandler.ThrowIFProgressAbort(m_Result);
				auto videoframe = vidEngine.VideoEngineGetFrame(i);
				if(videoframe == nullptr)
					throw std::exception("GetFrame error");
				ColorSpaceConverter csc{};
				csc.ConvertFrameColorFormat(m_pExporter, videoframe);
				//{ WRITEFRAME-this
				size_t written = 0;
				bool success = m_pExporter->EncodeVideo(videoframe, &written);
				if(success)
				{
					totalWritten += written;
					prgHandler.SetProgress(totalWritten);
					i += static_cast<__int64>(m_config.pPI->frameRate); //next iter
					delete videoframe;
				}
				else
				{
					delete videoframe;
					throw std::exception("Encode error");
				}
				//}
			}
			//}
			measure.Stop();
			LogExport(measure, m_config);
			m_Result = 1;
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
	return m_Result == 1;
}

bool ExportEngine::CheckBounceIsValid() const
{
	if(m_config.flagsExport & BOUNCE_IF_VALID and m_pExporter and m_config.pPI)
	{
		ExportConfig exConfig{};
		m_pExporter->GetExportInfo(&exConfig);
		return m_config.pPI->aspectRation == exConfig.aspectRatio //
			and m_config.pPI->width >= exConfig.width //
			and m_config.pPI->height >= exConfig.height //
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
		if(not(config.type == ExportType::DVD) or not (config.type == ExportType::MP4))
			throw std::exception("Feature not allowed");
	}
}
