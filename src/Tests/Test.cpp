#include "Test.h"
#include "../Utils/utils.h"
#include "../../../joson/JoSon.h"
#include "../World/WorldTime.h"

bool almostEquals(double d1, double d2) {
	double diff = abs(d1-d2);
	bool ok = diff < 0.001f;
	if (ok && diff > 0.000001f) {
		cout << "WARNING: assertion ok but response no so much accurate!!" << endl;
	}
	return ok;
}

void testF3d() {
	cout << "Starting testF3d..." << endl;

	// Test ctors
	f3d d3;
	assert(d3.x == 0 && d3.y == 0 && d3.z == 0);
	f3d d3b(1,2,3);
	assert(d3b.x == 1 && d3b.y == 2 && d3b.z == 3);

	// Test operators
	d3 = f3d(1,2,3) + 5.1;
	assert(d3.x == 6.1 && d3.y == 7.1 && d3.z == 8.1);

	d3 = f3d(1,2,3) + f3d(0.1,0.2,0.3);
	assert(d3.x == 1.1 && d3.y == 2.2 && d3.z == 3.3);

	d3 = f3d(1,2,3) - f3d(0.1,0.2,0.3);
	assert(d3.x == 0.9 && d3.y == 1.8 && d3.z == 2.7);

	d3 = f3d(1,2,3) * f3d(4,5,6);
	assert(d3.x == 4 && d3.y == 10 && d3.z == 18);

	d3 = f3d(1,2,3) / 2;
	assert(d3.x == 0.5 && d3.y == 1 && d3.z == 1.5);

	d3 = f3d(1,2,3) * 2.5;
	assert(d3.x == 2.5 && d3.y == 5 && d3.z == 7.5);

	// Test trigonometry
	f3d angle(PI/2, 0, PI/4);
	// Check cos
	f3d cosAngle = cos(angle);
	assert(almostEquals(cosAngle.x, 0));
	assert(almostEquals(cosAngle.y, 1));
	assert(almostEquals(cosAngle.z, 0.707106781));

	// Check sin
	f3d sinAngle = sin(angle);
	assert(almostEquals(sinAngle.x, 1));
	assert(almostEquals(sinAngle.y, 0));
	assert(almostEquals(sinAngle.z, 0.707106781));

	// Check tan
	f3d tanHalfAngle = tan(angle / 2);
	assert(almostEquals(tanHalfAngle.x, 1));
	assert(almostEquals(tanHalfAngle.y, 0));
	assert(almostEquals(tanHalfAngle.z, 0.414213562));

	// Check atan
	f3d resAtan = atan(f3d(1, 0, 0.414213562));
	assert(almostEquals(resAtan.x, PI/4));
	assert(almostEquals(resAtan.y, 0));
	assert(almostEquals(resAtan.z, PI/8));

	// Check sqrt
	f3d resSqrt = sqrt(f3d(9, 25, 100));
	assert(resSqrt.x == 3);
	assert(resSqrt.y == 5);
	assert(resSqrt.z == 10);

	cout << "testF3d OK" << endl;
}

void testJoSon() {
	cout << "Starting testJoSon..." << endl;

	string sError;
	JoSon * jsonPtr = JoSon::fromString(string("{a:1, b:[2, 3, \"toto et tata\"], c : foo, f:[9,8,7,8,9]}"), &sError);
	JoSon& json = *jsonPtr;

	assert(json.isMap());
	assert(json["a"].isLeaf());
	string a = json["a"].toString();
	assert(a == "1");
	assert(json["b"].isList());
	assert(json["b"][0].isLeaf());
	double b0 = json["b"][0].toDouble();
	assert(b0 == 2.0f);
	assert(json["b"][1].isLeaf());
	int b1 = json["b"][1].toInt();
	assert(b1 == 3);
	assert(json["b"][2].isLeaf());
	string b2 = json["b"][2].toString();
	assert(b2 == "toto et tata");
	assert(json["c"].isLeaf());
	string c = json["c"].toString();
	assert(c == "foo");
	assert(json["d"].isNull());
	string d = json["d"].toString();
	assert(d == "");
	assert(json["e"][9].isNull());
	int e = json["e"][9].toInt();
	assert(e == 0);

	// Extended list
	JoS_Union extended((JoS_List&)json["b"]);
	extended.concat((JoS_List&)json["f"])->concat((JoS_List&)json["b"]);
	assert(extended.size() == 11);
	assert(extended[0].toInt() == 2);
	assert(extended[5].toInt() == 7);
	assert(extended[10].toString() == "toto et tata");

	delete jsonPtr;
	cout << "testJoSon OK" << endl;
}

// Not actually a test... but detect if there are some similare traits, in which case it could be better to remove one.
void detectSimilarTraits()
{
    string jsonDesc = string(AI_PATH) + "traitsRelations.json";
    string err;
    JoSon * pTraitsRelations = JoSon::fromFile(jsonDesc, &err);
    assert(pTraitsRelations != NULL);
    JoSon& rel = *pTraitsRelations;

    // Compare each trait with each other to see if they are similar
	int nbTraits = rel["_list_"].size();
	for (int i = 0; i < nbTraits; i++) {
		string trait1 = rel["_list_"][i].toString();
		for (int j = i+1; j < nbTraits; j++) {
			string trait2 = rel["_list_"][j].toString();
			double totalSimilarity = 0.0f;
			int nbCounted = 0;
			for (int k = 0; k < nbTraits; k++) {
				string trait3 = rel["_list_"][k].toString();
				double i_k = rel[trait1][trait3].toDouble();
				double k_i = rel[trait3][trait1].toDouble();
				double j_k = rel[trait2][trait3].toDouble();
				double k_j = rel[trait3][trait2].toDouble();
				if (i_k != 0 || j_k != 0) {
					nbCounted++;
					double similarity = 1.0f - fabs(i_k - j_k);
					totalSimilarity += similarity;
				}
				if (k_i != 0 || k_j != 0) {
					nbCounted++;
					double similarity = 1.0f - fabs(k_i - k_j);
					totalSimilarity += similarity;
				}
			}
			if (nbCounted > 0) {
				int pct = (int)(100.0f*totalSimilarity/(float)nbCounted);
				if (pct >= 70) {
					cout << trait1 << " / " << trait2 << ": " << pct << endl;
				}
			}
		}
	}

    delete pTraitsRelations;
}

void testWorldTime()
{
	cout << "Starting testWorldTime..." << endl;

	DateTime now = _time->getTime();
	DateTime zero = now.minus(now);
	assert(zero.year == 0 && zero.month == 0 && zero.day == 0 && zero.hour == 0 && zero.minute == 0 && zero.second == 0);

	DateTime nowCopy = _time->getTimeCopy();
	nowCopy.add(3601);
	DateTime diff = nowCopy.minus(now);
	assert(diff.year == 0 && diff.month == 0 && diff.day == 0 && diff.hour == 1 && diff.minute == 0 && diff.second == 1);

	cout << "testWorldTime OK" << endl;
}

void runAllTests() {
	testF3d();
	testJoSon();
	testWorldTime();
	detectSimilarTraits();
}
