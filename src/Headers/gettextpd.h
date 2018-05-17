/*
 *      gettextpd.h
 *      Main file for GettextPD
 *
 *      © Copyright 2009 Compdigitec. All rights reserved.
 *
 *      Copying and distribution of this file, with or without modification,
 *      are permitted in any medium without royalty provided the copyright
 *      notice and this notice are preserved.  This file is offered as-is,
 *      without any warranty.
 */
#ifndef __GETTEXTPD__
#define __GETTEXTPD__
 
#include <stdlib.h>
#include <cstring>
#include <string>
#include <iostream>
#include <fstream>
 
// gettext functions
char* gettext (char *msgid);
std::string gettext (std::string msgid);
char* textdomain (const char *domain_name);
char* bindtextdomain (const char *domain_name, const char *dir_name);
char* setlocale (int category, const char* locale);
 
// macros
#define _(str) gettext(str)
 
// globals
std::string gtpd_locale = "";
std::string gtpd_domain = "";
std::string gtpd_location = "";
 
// function bodies
char* gettext(char* msgid)
{
    std::string x(msgid);
    std::string result;
    result = gettext(x);
    char* nb = (char*)malloc(sizeof(char)*result.length());
    strcpy(nb,result.c_str());
    return nb;
}
 
std::string gettext(std::string msgid)
{
    std::string path = gtpd_location + "/" + gtpd_locale + "/LC_MESSAGES/" + gtpd_domain + ".mo";
    std::ifstream stream;
    stream.open(path.c_str());
    std::string resultline;
    while(!stream.eof()) {
        getline(stream,resultline);
        if(strstr(resultline.c_str(),msgid.c_str()) != NULL) {
            break;
        }
        resultline = "";
    }
    if(resultline == "") {
        // not found
        return msgid;
    }
    int splitloc = resultline.find("\t");
    return resultline.substr(splitloc+1);
}
 
char* textdomain(const char *domain_name)
{
    gtpd_domain = domain_name;
    char* res = (char*)malloc(sizeof(char)*100);
    strcpy(res,domain_name);
    return res;
}
 
char* bindtextdomain (const char *domain_name, const char *dir_name)
{
    gtpd_location = dir_name;
    textdomain(domain_name);
    char* res = (char*)malloc(sizeof(char)*100);
    strcpy(res,dir_name);
    return res;
}
 
char* setlocale (int category, const char* locale)
{
    gtpd_locale = locale;
    char* res = (char*)malloc(sizeof(char)*100);
    strcpy(res,locale);
    return res;
}
 
#endif
