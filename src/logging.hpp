#pragma once
#include <fstream>

inline std::string getCurrentDateTime(std::string s) {
	time_t now = time(0);
	struct tm  tstruct;
	char  buf[80] = {};
	localtime_s(&tstruct, &now);
	if (s == "now")
		strftime(buf, sizeof(buf), "%Y-%m-%d %X", &tstruct);
	else if (s == "date")
		strftime(buf, sizeof(buf), "%Y_%m_%d", &tstruct);
	return std::string(buf);
};
inline void log(std::string logMsg) {
#ifdef  _DEBUG
	static std::string filePath = "log_" + getCurrentDateTime("date") + ".txt";
	std::string now = getCurrentDateTime("now");
	std::ofstream ofs(filePath.c_str(), std::ios_base::out | std::ios_base::app);
	std::string str = now + '\t' + logMsg;
	std::cout << str << std::endl;

	ofs << str << '\n';
	ofs.close();
#endif //  _DEBUG
}