#ifndef LINEAR_SARSA_AGENT
#define LINEAR_SARSA_AGENT

#include <iostream>
#include <sstream>
#include "SMDPAgent.h"
#include "tiles2.h"

#define RL_MEMORY_SIZE (2 << 20)
#define RL_MAX_NONZERO_TRACES (2 << 17)
#define RL_MAX_NUM_TILINGS (2 << 14)

class LinearSarsaAgent : public SMDPAgent {
protected:
  const string weightsFile;
  bool bLearning;
  bool bSaveWeights;
  bool jointTiling;

  /// Hive mind indicator and file descriptor.
  int hiveFile;

  double alpha;
  double gamma;
  double lambda;
  double epsilon;

  double tileWidths[MAX_STATE_VARS];
  double Q[MAX_ACTIONS];

  double *weights;
  double weightsRaw[RL_MEMORY_SIZE];

  int tiles[MAX_ACTIONS][RL_MAX_NUM_TILINGS];
  int numTilings;

  double minimumTrace;
  int numNonzeroTraces;

  double *traces;
  int *nonzeroTraces;
  int *nonzeroTracesInverse;

  double tracesRaw[RL_MEMORY_SIZE];
  int nonzeroTracesRaw[RL_MAX_NONZERO_TRACES];
  int nonzeroTracesInverseRaw[RL_MEMORY_SIZE];

  collision_table *colTab;

  // Load / Save weights from/to disk
  bool loadWeights(const char *filename);

  bool saveWeights(const char *filename);

  // Value function methods for CMACs
  int selectAction(double state[]);

  double computeQ(int a);

  int argmaxQ(double state[]) const;

  void updateWeights(double delta);

  virtual void loadTiles(double state[]);

  // Eligibility trace methods
  void clearTrace(int f);

  void clearExistentTrace(int f, int loc);

  void decayTraces(double decayRate);

  void setTrace(int f, float newTraceValue);

  void increaseMinTrace();

  size_t mmapSize();

  void loadSharedData(double *weights);

  void saveSharedData(double *weights);
  virtual void sync(bool load);

  double reward(double tau, double gamma);

public:
  LinearSarsaAgent(int numFeatures,
                   bool bLearn,
                   double widths[],
                   std::string loadWeightsFile,
                   std::string saveWeightsFile,
                   int hiveMind,
                   bool jointTiling,
                   double gamma);

  void setEpsilon(double epsilon);

  // SMDP Sarsa implementation
  int startEpisode(int current_time, double state[]);

  int step(int current_time, double reward_, double state[]);

  void endEpisode(int current_time, double reward_);

  void setParams(int iCutoffEpisodes, int iStopLearningEpisodes);

  void shutDown();
};

#endif
