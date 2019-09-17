#ifndef DOCUMMENTFACTORY_H
#define DOCUMMENTFACTORY_H
#include "docummentbase.h"
#include "docummentproxy.h"
#include <DWidget>

DWIDGET_USE_NAMESPACE
class DocummentFactory
{
public:
    DocummentFactory();

    static DocummentBase *creatDocumment(DocType_EM type, DWidget *father);

};

#endif // DOCUMMENTFACTORY_H
