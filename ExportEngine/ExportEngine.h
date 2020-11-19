#pragma once
#include <string>
#include "ExportData.h"
#include "ExportHandler.h"


class ExportEngine
{
public:
	bool Bounce(const ExportEngineConfig&);

private:
	bool CheckBounceIsValid(const ExportHandler&) const;

	ExportEngineConfig m_config = {};
	int m_Result = 0;
	IVideoExport* m_pExporter = nullptr;
};
