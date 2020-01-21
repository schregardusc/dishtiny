#pragma once

#include <float.h>
#include <string>

#include "config/config.h"

EMP_BUILD_CONFIG(
  ConfigBase,

  GROUP(CORE, "Core settings for DISHTINY"),
  VALUE(NLEV, size_t, 2, "Number of hierarchical resource levels."),
  VALUE(SEED, int, 1, "Random number generator seed."),
  VALUE(GRID_H, size_t, 60, "How many tiles tall should the grid be?"),
  VALUE(GRID_W, size_t, 60, "How many tiles wide should the grid be?"),
  VALUE(RESOURCE_DECAY, double, 0.995, "How much resource should remain each update?"),
  VALUE(REP_THRESH, double, 1.0, "How much should replication cost?"),
  VALUE(REP_LEAD, double, 0.0, "How more more resource than your target do you need to trample them?"),
  VALUE(START_RESOURCE, double, 40.0, "How much resource should a cell start with?"),
  VALUE(KILL_THRESH, double, -1.25, "What is the minimum resource level required to stay alive?"),
  VALUE(APOP_RECOVERY_FRAC, double, 0.5, "What fraction of REP_THRESH is recovered to heirs after apoptosis?"),
  VALUE(BASE_RESOURCE_INFLOW, double, 0.0051, "What amount of resource should be provided to cells at each update?"),
  VALUE(AGE_LIMIT_MULTIPLIER, double, 1.5, "What ratio of EVENT_RADIUS should the limit on cell age be?"),
  VALUE(CHANNELS_VISIBLE, bool, true, "Should channels have any effect in the instruction set and event triggers?"),
  VALUE(GEN_INCR_FREQ, size_t, 512, "How often should we increase cell generation counters?"),
  VALUE(EXP_GRACE_PERIOD, double, 3.0, "How many channel generations should resource collection be allowed after a cell's expires channel generation counter?"),
  VALUE(STOCHASTIC_TRIGGER_FREQ, double, 0.001, "How often should the stochastic event fire?"),
  VALUE(WAVE_REPLICATES, size_t, 4, "How many replicates of the wave system should operate concurrently?"),
  VALUE(RESOURCE_ARMOR, bool, false, "Should cells be able to use stockpiled resource to block incoming reproduction?"),
  VALUE(LOCAL_REPRODUCTION, bool, true, "Should cell reproductions be targeted to neighbor cells?"),
  VALUE(RUN_SPIKER, bool, true, "Should the spiker cpu be stepped?"),
  VALUE(MAX_CONNECTIONS, size_t, 3, "How many cell-cell connections should be allowed outgoing per cell?"),
  VALUE(FLEDGLING_COPIES, size_t, 4, "How many copies of fledgling connections should be gemerated?"),

  GROUP(PROGRAM_GROUP, "SignalGP program Settings"),
  VALUE(PROGRAM_MAX_FUN_CNT__INIT, size_t, 16, "Used for generating SGP programs. At most, how many functions do we generate?"),
  VALUE(PROGRAM_MIN_FUN_CNT__INIT, size_t, 4, "Used for generating SGP programs. At least, how many functions do we generate?"),
  VALUE(PROGRAM_MAX_FUN_LEN__INIT, size_t, 16, "Used for generating SGP programs. At most, for each function how many instructions do we generate?"),
  VALUE(PROGRAM_MIN_FUN_LEN__INIT, size_t, 1, "Used for generating SGP programs. At least, for each function how many instructions do we generate?"),
  VALUE(PROGRAM_MIN_ARG_VAL__INIT, int, 0, "Minimum argument value for instructions."),
  VALUE(PROGRAM_MAX_ARG_VAL__INIT, int, 4, "Maximum argument value for instructions."),

  GROUP(HARDWARE_GROUP, "SignalGP Hardware Settings"),
  VALUE(HARDWARE_STEPS, size_t, 16, "How many hardware steps to run per update?"),
  VALUE(ENVIRONMENT_SIGNALS, bool, true, "Can environment signals trigger functions?"),
  VALUE(ACTIVE_SENSORS, bool, true, "Do agents have function active sensors?"),
  VALUE(HW_MAX_CORES, size_t, 24, "Max number of hardware cores; i.e., max number of simultaneous threads of execution hardware will support."),
  VALUE(HW_MAX_CALL_DEPTH, size_t, 128, "Max call depth of a hardware unit."),
  VALUE(INBOX_CAPACITY, size_t, 16, "Capacity of a cell's messaging inbox."),
  VALUE(ENV_TRIG_FREQ, size_t, 8, "How often to fire environmental trigger events?"),
  VALUE(COMPUTE_FREQ, size_t, 4, "How often to step the CPUs?"),

  GROUP(MUTATION_GROUP, "SignalGP Mutation Settings"),
  VALUE(MUTATION_RATE, double, 0.01, "What percentage of offspring should experience mutations?"),
  VALUE(PROPAGULE_MUTATION_RATE, double, 0.0, "What percentage of propagule offspring should experience additional mutations?"),
  VALUE(PROGRAM_MIN_ARG_VAL, int, 0, "Minimum argument value for instructions."),
  VALUE(PROGRAM_MAX_ARG_VAL, int, 8, "Maximum argument value for instructions."),
  VALUE(TAG_BIT_FLIP__PER_BIT, double, 0.005, "Per-bit mutation rate of tag bit flips."),
  VALUE(INST_SUB__PER_INST, double, 0.005, "Per-instruction/argument subsitution mutation rate."),
  VALUE(ARG_SUB__PER_ARG, double, 0.005, "Per-instruction/argument subsitution mutation rate."),
  VALUE(INST_INS__PER_INST, double, 0.005, "Per-instruction insertion mutation rate."),
  VALUE(INST_DEL__PER_INST, double, 0.005, "Per-instruction deletion mutation rate."),
  VALUE(SLIP__PER_FUNC, double, 0.05, "Per-function rate of slip mutations."),
  VALUE(FUNC_DUP__PER_FUNC, double, 0.01, "Per-function rate of function duplication mutations."),
  VALUE(FUNC_DEL__PER_FUNC, double, 0.01, "Per-function rate of function deletion mutationss."),
  VALUE(PROGRAM_MAX_FUN_CNT, size_t, 512, "Used for mutating SGP programs. At most, how many functions can we have?"),
  VALUE(PROGRAM_MIN_FUN_CNT, size_t, 1, "Used for mutating SGP programs. At least, how many functions can we have?"),
  VALUE(PROGRAM_MAX_FUN_LEN, size_t, 64, "Used for mutating SGP programs. At most, for each function how many instructions can we have?"),
  VALUE(PROGRAM_MIN_FUN_LEN, size_t, 1, "Used for mutating SGP programs. At least, for each function how many instructions can we have?"),

  GROUP(LOGISTICS, "logistics"),
  VALUE(TREATMENT_DESCRIPTOR, std::string, "unspecified", "[NATIVE] Treatment identifying slug"),
  VALUE(CONFIGLEVEL_BASENAME, std::string, "level=", "[NATIVE] Base filename"),
  VALUE(CONFIGLEVEL_EXTENSION, std::string, ".cfg", "[NATIVE] Filename extension"),
  VALUE(SNAPSHOT_FREQUENCY, size_t, 262144, "[NATIVE] How often should we save data snapshots?"),
  VALUE(SNAPSHOT_LENGTH, size_t, 8192, "[NATIVE] How long should snapshots last for?"),
  VALUE(RUN_LENGTH, size_t, 1056768, "[NATIVE] How many updates should we run the experiment for?"),
  VALUE(SYSTEMATICS, bool, true, "[NATIVE] Should we keep systematics data?"),
  VALUE(SEED_POP, bool, false, "[NATIVE] Should we seed the population?"),
  VALUE(SEED_POP_ID, size_t, 0, "[NATIVE] Should we seed the population with all seedpop IDs (0) or with a specific ID (>0)?"),
  VALUE(ANIMATION_FREQUENCY, size_t, 512, "[NATIVE] How often should we save data that will be used to make animations?")

)
