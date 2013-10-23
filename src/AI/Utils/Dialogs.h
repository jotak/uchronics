#ifndef _UTILS_DIALOGS_H
#define _UTILS_DIALOGS_H

#include "../../Data/JSonUtil.h"

class Dialogs
{
public:
    // Destructor
    ~Dialogs();
    static Dialogs * getInstance()
    {
        if (m_pInstance == NULL) m_pInstance = new Dialogs();
        return m_pInstance;
    };

    void aggregateTemplates(JoS_Union& dialogs, JoS_Element& refTpl);

private:
    Dialogs();
    static Dialogs * m_pInstance;
    JoSon * loadJson(string path);

    JoSon * templateNames;
};

// Helper alias
#define _dlg Dialogs::getInstance()

#endif
