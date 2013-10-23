// -----------------------------------------------------------------
// JSON_UTIL
// -----------------------------------------------------------------
#include "JSonUtil.h"

// -----------------------------------------------------------------
// Name : getDouble
// -----------------------------------------------------------------
double JSonUtil::getDouble(const JoS_Element& elt, double defaultVal)
{
	if (elt.isList() && elt[0].isLeaf() && elt[1].isLeaf()) {
		// Case list of 2 elements => random in min/max
		double min = elt[0].toDouble();
		double max = elt[1].toDouble();
		return min + FRAND100(max-min);
	} else if (elt.isLeaf()) {
		return elt.toDouble();
	} else {
		return defaultVal;
	}
}

// -----------------------------------------------------------------
// Name : getString
// -----------------------------------------------------------------
string JSonUtil::getString(const JoS_Element& elt, string defaultVal)
{
	if (elt.isList()) {
		// Enumeration of strings => take random one
		int idx = rand() % elt.size();
		return elt[idx].toString();
	} else if (elt.isLeaf()) {
		return elt.toString();
	} else {
		return defaultVal;
	}
}

// -----------------------------------------------------------------
// Name : getInt
// -----------------------------------------------------------------
int JSonUtil::getInt(const JoS_Element& elt, int defaultVal)
{
	if (elt.isList() && elt[0].isLeaf() && elt[1].isLeaf()) {
		// Case list of 2 elements => random in min/max
		int min = elt[0].toInt();
		int max = elt[1].toInt();
		return min + rand() % (1+max-min);
	} else if (elt.isLeaf()) {
		return elt.toInt();
	} else {
		return defaultVal;
	}
}

// -----------------------------------------------------------------
// Name : getCappedInt
// -----------------------------------------------------------------
int JSonUtil::getCappedInt(const JoS_Element& elt, int min, int max, int defaultVal)
{
	int val = getInt(elt, defaultVal);

	// Min/max capping
	if (val < min) {
		val = min;
	} else if (val > max) {
		val = max;
	}
	return val;
}
