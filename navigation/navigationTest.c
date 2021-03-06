#include "navigationTest.h"

/**
 * Navigation Location and Path tests from wikipedia
 * http://fr.wikipedia.org/wiki/Algorithme_de_Dijkstra
 */

#include "location.h"
#include "locationList.h"
#include "path.h"
#include "pathList.h"
#include "navigation.h"

#include "../common/string/cenString.h"

#include "../common/io/outputStream.h"
#include "../common/io/printWriter.h"

#include "../common/log/logger.h"
#include "../common/log/logLevel.h"

static Location locationA;
static Location locationB;
static Location locationC;
static Location locationD;
static Location locationE;
static Location locationF;
static Location locationG;
static Location locationH;
static Location locationI;
static Location locationJ;

static LocationList resultLocationList;

int addNavigationLocations() {
	initNavigation();

	// locations
	addNavigationLocation(&locationA, "A", 0, 0);
	addNavigationLocation(&locationB, "B", 0, 0);
	addNavigationLocation(&locationC, "C", 0, 0);
	addNavigationLocation(&locationD, "D", 0, 0);
	addNavigationLocation(&locationE, "E", 0, 0);
	addNavigationLocation(&locationF, "F", 0, 0);
	addNavigationLocation(&locationG, "G", 0, 0);
	addNavigationLocation(&locationH, "H", 0, 0);
	addNavigationLocation(&locationI, "I", 0, 0);
	addNavigationLocation(&locationJ, "J", 0, 0);

	// paths
	void pathABFunction() { fillPathData(&locationA, &locationB, 85, 0, 0, 0, 0, 0, 0); }
	addNavigationPath(&pathABFunction);
	void pathBFFunction() { fillPathData(&locationB, &locationF, 80, 0, 0, 0, 0, 0, 0); }
	addNavigationPath(&pathBFFunction);
	void pathFIFunction() { fillPathData(&locationF, &locationI, 250, 0, 0, 0, 0, 0, 0); }
	addNavigationPath(&pathFIFunction);
	void pathIJFunction() { fillPathData(&locationI, &locationJ, 84, 0, 0, 0, 0, 0, 0); }
	addNavigationPath(&pathIJFunction);

	void pathACFunction() { fillPathData(&locationA, &locationC, 217, 0, 0, 0, 0, 0, 0); }
	addNavigationPath(&pathACFunction);
	void pathCGFunction() { fillPathData(&locationC, &locationG, 186, 0, 0, 0, 0, 0, 0); }
	addNavigationPath(&pathCGFunction);

	void pathCHFunction() { fillPathData(&locationC, &locationH, 103, 0, 0, 0, 0, 0, 0); }
	addNavigationPath(&pathCHFunction);
	void pathHDFunction() { fillPathData(&locationH, &locationD, 183, 0, 0, 0, 0, 0, 0); }
	addNavigationPath(&pathHDFunction);
	void pathHJFunction() { fillPathData(&locationH, &locationJ, 167, 0, 0, 0, 0, 0, 0); }
	addNavigationPath(&pathHJFunction);

	void pathEJFunction() { fillPathData(&locationE, &locationJ, 502, 0, 0, 0, 0, 0, 0); }
	addNavigationPath(&pathEJFunction);
	void pathAEFunction() { fillPathData(&locationA, &locationE, 173, 0, 0, 0, 0, 0, 0); }
	addNavigationPath(&pathAEFunction);

	printPathList(getOutputStreamLogger(INFO), "Paths definition", getNavigationPathList());

	int cost = computeBestPath(&resultLocationList, &locationA, &locationJ);

	printLocationList(getOutputStreamLogger(INFO), "Result=", &resultLocationList);

	return cost;
}
