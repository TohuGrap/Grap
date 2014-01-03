
#ifndef XML_UTLS_H_
#define XML_UTLS_H_
#include <libxml/tree.h>
class XmlUtls {
public:
  static int GetIntContent(xmlDocPtr doc, xmlNodePtr cur);
  static double GetDoubleContent(xmlDocPtr doc, xmlNodePtr cur);
  static std::wstring GetStringContent(xmlDocPtr doc, xmlNodePtr cur);
  static COLORREF GetColorREFContent(xmlDocPtr doc, xmlNodePtr cur);
  static bool GetBooleanContent(xmlDocPtr doc, xmlNodePtr cur);
};
#endif // XML_UTLS_H_
