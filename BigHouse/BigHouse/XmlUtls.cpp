
#include <libxml/xmlwriter.h>
#include "stdafx.h"
#include "XmlUtls.h"

bool XmlUtls::GetBooleanContent(xmlDocPtr doc, xmlNodePtr cur) {
  xmlChar* str = xmlNodeListGetString(doc, cur->children, 1);
  bool ret;
  if (!xmlStrcmp(str, (const xmlChar*)"true"))
    ret = true;
  else 
    ret = false;
  xmlFree(str);
  return ret;
}

COLORREF XmlUtls::GetColorREFContent(xmlDocPtr doc, xmlNodePtr cur) {
  xmlChar* str = xmlNodeListGetString(doc, cur->children, 1);
  COLORREF clr;
  sscanf_s((const char*)str, "%ld", &clr);
  xmlFree(str);
  return clr;
}

double XmlUtls::GetDoubleContent(xmlDocPtr doc, xmlNodePtr cur) {
  xmlChar* str = xmlNodeListGetString(doc, cur->children, 1);
  double value = (double)atof((const char*)str);
  xmlFree(str);
  return value;
}

std::wstring XmlUtls::GetStringContent(xmlDocPtr doc, xmlNodePtr cur) {
  xmlChar * str = xmlNodeListGetString(doc, cur->children, 1);
  std::string st((char*)str);
  std::wstring wstr(st.begin(), st.end());
  xmlFree(str);
  return wstr;
}

int XmlUtls::GetIntContent(xmlDocPtr doc, xmlNodePtr cur) {
  xmlChar* str = xmlNodeListGetString(doc, cur->children, 1);
  int value = atoi((const char*)str);
  xmlFree(str);
  return value;
}
