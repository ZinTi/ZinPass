#include "TelecomOperatorService.h"
#include "TelecomOperatorDAO.h"
#include "Environ.h"

extern Environ* ZinPassEnv;

bool TelecomOperatorService::getTelecomOperatorOptions(std::string& msg, std::vector<std::string>& telecomOperator){
    bool result;
    if (
        TelecomOperatorDAO teleOperDAO(ZinPassEnv->getDbPath());
        teleOperDAO.findList(msg, telecomOperator)
    ) {
        result = true;
    }
    else {
        result = false;
    }
    return result;
}