#ifndef ISINPUTVALID_H
#define ISINPUTVALID_H

#include <QDialog>
#include <QDateTime>

// 0. 注册用户界面
bool isPasswordValid(const QString& password1, const QString& password2);
bool isUserIdValid(const QString& userId);
bool isAliasValid(const QString& alias);

// 1. 主工作台界面
// (1.1) 账号记录查询-筛选控件
bool isUTimeRangeValid(const QDateTime& dateRangeStart, const QDateTime& dateRangeEnd);

#endif
