/* Copyright (c) 2005-2013 Brocade Communications Systems, Inc.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at:
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef _PORT_CMD_H
#define _PORT_CMD_H
#ifdef __cplusplus
extern "C"
{
#endif

#define PORT_TH_CONFIG "portThConfig"
#define PORT_FENCING  "portFencing"
#define SYS_MONITOR   "sysMonitor"
#define TH_CONFIG	"thConfig"
#define PORT_STATS	"portStats"
#define TH_MONITOR "thMonitor"

#define PORT_FENCING_BITMAP  0x00000020
#define MAX_ACTION_LEN	100
#define FW_BKEND_FAILED  -1
#define MAX_NO_ACTION 5
enum {
	APPLY,
	DONT_APPLY
};
enum {
	FW_ACTION_NONE		= 0,
	FW_ACTION_RASLOG	= 1,
	FW_ACTION_SNMP		= 2,
	FW_ACTION_PORTLOG	= 4,
	FW_ACTION_EMAIL		= 16,
	FW_ACTION_HA_REBOOT = 64
};
typedef struct actionMapping {
	int act;
	char *name;
}actionMapping_t;

typedef struct timeEnum {
	char *name;
	int value;
} timeMapping_t;

extern timeMapping_t gTImeEnumMap[];
extern actionMapping_t gActions[];


struct cliOperands {
	char operand[256];

	int pollInt;
	int pollIntFound;

	int retries;
	int retriesFound;

	int usage;
	int usageFound;

	int  action;
	int actionFound;

	int lowUsageLimit;
	int lowUsageFound;

	int highUsageLimit;
	int highUsageFound;

};
struct th  {

	int boundry;
	int boundryFound;

	int val;
	int valFound;

	char actions[MAX_ACTION_LEN];
	int actionFound;
};
struct threshold {
	char area;
	int areaFound;

	int timebase;
	int timeBaseFound;

	struct th highTh;
	int highThFound;

	struct th lowTh;
	int lowThFound;

	struct th buffer;
	int bufferFound;

	int save;
	int saveFound;

	char oui[MAX_OUI_LEN];
	int ouiFound;
};
#define	TH_RAW_HIST_ENTRIES	60
struct fwPortStats 
{
	u_int    portIndex;
	u_int	 linkFailure;
	u_int	 lossOfSync;
	u_int	 lossOfSig;
	u_int    protoErr;
	u_int	 itw;
	u_int	 crc;
	u_int    lrIn;
	u_int    lrOut;
	u_int    stChange;
	u_int    c3TxTo;
};

struct fwPortStatsRec 
{
	unsigned long long int cnt;
	int index;
	int port;
	time_t pTime;
	struct fwPortStats max[3];
	struct fwPortStats avg;
	struct fwPortStats stats[TH_RAW_HIST_ENTRIES];
};

typedef struct 
{
	int statsType;
	int statsTypeFound;
	int pIndex; // port index
	int pIndexFound;
	int portType;
	int portTypeFound;
	int opType;
	int ret;
	int read;
	struct fwPortStatsRec data;
} statsStruct_t;

int fwCpuOrMemUsageHandler(struct pcbExtInterface *cfg,  OPS_T, USAGE_T);
void printPcb(struct pcbExtInterface *ptr);
//int showUsage(char *ptr);
int fwParseCpuUsageCLICommand(int argc,  char ** buffer);
void printNode();
void initializeValues();
void fwExecuteCpuUsage(int argc, char **argv);
int fwHandleCpuUtilShowCommand(int argc, char **argv);

int fwHandleCpuUtilConfigCommand(struct cliOperands *ptr);
void fwExecuteMemoryUsage(int argc, char **argv);
char *getAction(int act);

void portFencing(int argc, char *argv[]);
void portThConfig(int argc, char *argv[]);
//int showUsage(char *ptr);
int fwHandlePortFencingShow(int argc, char *argv[], int class1);
int getClassEnum(char *className);
int fwHandleVEPortThConfigShow(int argc, char *argv[], char class1);

int fwGetAction(char *actions, int class1);
int fwMapThreshold(int val, char *timebase, char *buffer1, char *buffer2);
void fwMapActions(int act, char *buffer, char *buffer2);
int getBoundry(char *state);
int getTimeEnum(char *timeBase);
void convertTimeBase(int tb, char *timebase);
char *fwStateStr(int state);
char *fwStatusStr(int status);
void portFencing(int argc, char *argv[]);
void portThConfig(int argc, char *argv[]);
int fwHandlePortFencingEnableOrDisable(char *areaBuf, int class1, int enableOrDisable, int *classflag);
int fwHandleVEPortThConfigShow(int argc, char *argv[], char class1);
int fwSetPortThConfig(int class1, int pArea, struct threshold *ptr);
int fwGetActionLevel(char *optarg);
int fwGetThLevel(char *optarg);
int validateHighThValue(int highThVal);
int validateThStruct(struct threshold * ptr, int class1, int area);
void printThStruct(struct threshold *ptr);
int fwCancelPortThConfig(int argc, char *argv[], int class1);
char *lowerString(char *name);
void thConfig(int argc, char *argv[]);
void thMonitor(int argc, char *argv[]);
int unsupportedTimeBaseClass(int class_name, int area_name);
int unsupportedClassArea(int class_name, int area_name);
int supportedClasses(char *cmdName, int class_name);
int fwHandleCmdThConfigShow(int argc, char *argv[], int class1, char *cmdHelp);
int fwSetCmdThConfig(int class1, int pArea, struct threshold *ptr);
int fwParseCmdThConfig(int argc, char *argv[], int class1, int *area,
							struct threshold *ptr, char *cmd);
int fwApplyCmdThConfig(int argc, char *argv[], int class1, char *cmd);
int fwCancelCmdThConfig(int argc, char *argv[], int class1, char *cmd);
int fwPauseContinueCmdThConfig(int argc, char *argv[], int class1, char *cmd,
								int pflag);
int checkIsDigit(char *str, int class1, int area);
int fwCheckForModelRestriction(int class1);
void printStats(statsStruct_t *stats);
int fwPfEnableOrDisableArea(int class1, int area, int enableOrDisable);
int fwParseClassOption(char *areaLst, int *optArray, int *numOpts);
int fwParseAreaOption(char *optLst, int *optArray, int *numOpts, int class1);
int validate_token(char *token);
int getSlotPortIndex(char *slotPort, int *indexPort);
int fwParseArea(char *optLst, int *areaArray, int *numAreas, int class1);
int fwParsePortList(char *optList, int *portArray, int *numPorts);
#ifdef __cplusplus
}
#endif

#endif
