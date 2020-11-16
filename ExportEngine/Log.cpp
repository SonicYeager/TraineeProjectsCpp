#include "Log.h"

void Log::LogFileName(const ExportEngineConfig& config)
{
	std::cout << "Export" << config.targetFileName;
}

void Log::LogRange(const ExportEngineConfig& config)
{
	if(config.pPI->rangeEnd > config.pPI->rangeStart)
		std::cout << " from " << config.pPI->rangeStart << " to " << config.pPI->rangeEnd << " started.\n";
}

void Log::LogExport(const Measurement& measure, const ExportEngineConfig& config)
{
	const double expLen = config.pPI->rangeEnd - config.pPI->rangeStart / config.pPI->frameRate;
	std::cout << "Export " << std::fixed << std::setprecision(1) << expLen
			  << "s finished successful (Duration=" << measure.GetPassesTime() << " ms)\n";
}
