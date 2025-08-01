#ifndef PREDICTOR_H
#define PREDICTOR_H
#include "utils.h"

namespace cpu_sim {
  class Predictor {
  public:
    virtual ~Predictor() = default;

    virtual bool predict(u32 pc) const = 0;

    virtual void update(u32 pc, int taken) = 0;
  };

  class NaivePredictor final : public Predictor {
  public:
    NaivePredictor();

    bool predict(u32 pc) const override;

    void update(u32 pc, int taken) override;
  };

  class TwoBitsPredictor final : public Predictor {
  public:
    int log[1024] = {};

    TwoBitsPredictor();

    bool predict(u32 pc) const override;

    void update(u32 pc, int taken) override;
  };

  class GlobalPredictor final : public Predictor {
  public:
    int log[1024] = {};
    u32 history = 0;

    GlobalPredictor();

    bool predict(u32 pc) const override;

    void update(u32 pc, int taken) override;
  };

  class LocalPredictor final : public Predictor {
  public:
    int log[1024] = {};
    u32 local_history[1024] = {};

    LocalPredictor();

    bool predict(u32 pc) const override;

    void update(u32 pc, int taken) override;
  };

  class ChoicePredictor {
  public:
    int choice[1024] = {};

    ChoicePredictor();

    ~ChoicePredictor();

    bool prefer(u32 pc) const;

    void update(u32 pc, bool local_correct, bool global_correct);
  };

  class TournamentPredictor : public Predictor {
  public:
    GlobalPredictor global_predictor;
    LocalPredictor local_predictor;
    ChoicePredictor choice_predictor;

    TournamentPredictor();

    bool predict(u32 pc) const override;

    void update(u32 pc, int taken) override;
  };

  extern NaivePredictor naive;

  extern TwoBitsPredictor two_bits;

  extern GlobalPredictor global;

  extern LocalPredictor local;

  extern TournamentPredictor tournament;
}


#endif //PREDICTOR_H
