// -----------------------------------------------------------------
// Common dialogs
// -----------------------------------------------------------------
#include "Dialogs.h"
#include "../../Managers/DebugManager.h"

Dialogs * Dialogs::m_pInstance = NULL;

// -----------------------------------------------------------------
// Name : Dialogs
// -----------------------------------------------------------------
Dialogs::Dialogs()
{
	templateNames = loadJson("templates.json");
}

// -----------------------------------------------------------------
// Name : ~Dialogs
// -----------------------------------------------------------------
Dialogs::~Dialogs()
{
	delete templateNames;
}

// -----------------------------------------------------------------
// Name : aggregateTemplates
//	dialogs is the current loaded dialogs
//	refTpl is a template name that point to new dialogs to aggregate
//	Member variable templateNames is a map <tpl_name(string),files_to_load(list)>
//	For instance: "common: [idle, buying]"
//	Each file to load contains dialogs to be aggregated.
// -----------------------------------------------------------------
void Dialogs::aggregateTemplates(JoS_Union& dialogs, JoS_Element& refTpl)
{
	if (refTpl.isLeaf()) {
		JoS_Element& listToAggregate = (*templateNames)[refTpl.toString()];
		if (listToAggregate.isList()) {
			int len = listToAggregate.size();
			// For each template to load
			for (int iTpl = 0; iTpl < len; iTpl++) {
				JoS_Element& tpl = listToAggregate[iTpl];
				if (tpl.isLeaf()) {
					JoSon * dialogsToAggregate = loadJson(tpl.toString() + ".json");
					dialogs.concat(*dialogsToAggregate);
				}
			}
		}
	}
}

// -----------------------------------------------------------------
// Name : loadJson
// -----------------------------------------------------------------
JoSon * Dialogs::loadJson(string path)
{
    // Build file name
    string jsonDesc = string(DIALOGS_PATH) + path;
    string err;
    JoSon * json = JoSon::fromFile(jsonDesc, &err);
    if (json == NULL) {
    	_debug->error(err);
    }
    return json;
}
