#ifndef APPS_CONTAINER_H
#define APPS_CONTAINER_H

#include <escher/blink_timer.h>
#include <escher/container.h>
#include <ion/events.h>

#include "apps_window.h"
#include "backlight_dimming_timer.h"
#include "battery_timer.h"
#include "empty_battery_window.h"
#include "exam_pop_up_controller.h"
#include "hardware_test/app.h"
#include "home/app.h"
#include "on_boarding/app.h"
#include "shared/global_context.h"
#include "shared/prompt_controller.h"
#include "suspend_timer.h"
#include "usb/app.h"

#include "apps/calculation/app.h"
#include "apps/graph/app.h"
#include "apps/code/app.h"
#include "apps/statistics/app.h"
#include "apps/distributions/app.h"
#include "apps/inference/app.h"
#include "apps/solver/app.h"
#include "apps/sequence/app.h"
#include "apps/regression/app.h"
#include "apps/elements/app.h"
#include "apps/finance/app.h"
#include "apps/settings/app.h"
#include "apps/rpn/app.h"

#define APPS_CONTAINER_SNAPSHOT_COUNT 13

#define APPS_CONTAINER_SNAPSHOT_LIST &m_snapshotCalculationAppSnapshot, \
                            &m_snapshotGraphAppSnapshot, \
                            &m_snapshotCodeAppSnapshot, \
                            &m_snapshotStatisticsAppSnapshot, \
                            &m_snapshotDistributionsAppSnapshot, \
                            &m_snapshotInferenceAppSnapshot, \
                            &m_snapshotSolverAppSnapshot, \
                            &m_snapshotSequenceAppSnapshot, \
                            &m_snapshotRegressionAppSnapshot, \
                            &m_snapshotElementsAppSnapshot, \
                            &m_snapshotFinanceAppSnapshot, \
                            &m_snapshotSettingsAppSnapshot,             \
                            &m_snapshotRpnAppSnapshot

#define APPS_CONTAINER_SNAPSHOT_CONSTRUCTORS m_snapshotCalculationAppSnapshot(), \
                            m_snapshotGraphAppSnapshot(), \
                            m_snapshotCodeAppSnapshot(), \
                            m_snapshotStatisticsAppSnapshot(), \
                            m_snapshotDistributionsAppSnapshot(), \
                            m_snapshotInferenceAppSnapshot(), \
                            m_snapshotSolverAppSnapshot(), \
                            m_snapshotSequenceAppSnapshot(), \
                            m_snapshotRegressionAppSnapshot(), \
                            m_snapshotElementsAppSnapshot(), \
                            m_snapshotFinanceAppSnapshot(), \
                            m_snapshotSettingsAppSnapshot(),                     \
                            m_snapshotRpnAppSnapshot()

#define APPS_CONTAINER_SNAPSHOT_DECLARATIONS \
                            Calculation::App::Snapshot m_snapshotCalculationAppSnapshot; \
                            Graph::App::Snapshot m_snapshotGraphAppSnapshot; \
                            Code::App::Snapshot m_snapshotCodeAppSnapshot; \
                            Statistics::App::Snapshot m_snapshotStatisticsAppSnapshot; \
                            Distributions::App::Snapshot m_snapshotDistributionsAppSnapshot; \
                            Inference::App::Snapshot m_snapshotInferenceAppSnapshot; \
                            Solver::App::Snapshot m_snapshotSolverAppSnapshot; \
                            Sequence::App::Snapshot m_snapshotSequenceAppSnapshot; \
                            Regression::App::Snapshot m_snapshotRegressionAppSnapshot; \
                            Elements::App::Snapshot m_snapshotElementsAppSnapshot; \
                            Finance::App::Snapshot m_snapshotFinanceAppSnapshot; \
                            Settings::App::Snapshot m_snapshotSettingsAppSnapshot;       \
                            Rpn::App::Snapshot m_snapshotRpnAppSnapshot;

#define APPS_CONTAINER_APPS_DECLARATION \
                            Calculation::App m_CalculationApp; \
                            Graph::App m_GraphApp; \
                            Code::App m_CodeApp; \
                            Statistics::App m_StatisticsApp; \
                            Distributions::App m_DistributionsApp; \
                            Inference::App m_InferenceApp; \
                            Solver::App m_SolverApp; \
                            Sequence::App m_SequenceApp; \
                            Regression::App m_RegressionApp; \
                            Elements::App m_ElementsApp; \
                            Finance::App m_FinanceApp; \
                            Settings::App m_SettingsApp;       \
                            Rpn::App nRpnApp;

class AppsContainer : public Escher::Container, Ion::Storage::StorageDelegate {
 public:
  static AppsContainer* sharedAppsContainer();
  AppsContainer();
  virtual int numberOfBuiltinApps() = 0;
  int numberOfApps() { return numberOfExternalApps() + numberOfBuiltinApps(); }
  int numberOfExternalApps();
  virtual Escher::App::Snapshot* appSnapshotAtIndex(int index) = 0;
  Ion::ExternalApps::App externalAppAtIndex(int index);
  Escher::App::Snapshot* initialAppSnapshot();
  Escher::App::Snapshot* hardwareTestAppSnapshot();
  Escher::App::Snapshot* onBoardingAppSnapshot();
  Escher::App::Snapshot* usbConnectedAppSnapshot();
  Home::App::Snapshot* homeAppSnapshot() { return &m_homeSnapshot; }
  void setExamMode(Poincare::ExamMode targetExamMode,
                   Poincare::ExamMode previousMode);
  Shared::GlobalContext* globalContext();
  void didSuspend();
  bool dispatchEvent(Ion::Events::Event event) override;
  void switchToBuiltinApp(Escher::App::Snapshot* snapshot) override;
  void switchToExternalApp(Ion::ExternalApps::App app);
  void run() override;
  bool updateBatteryState();
  void refreshPreferences();
  void reloadTitleBarView();
  void displayExamModePopUp(Poincare::ExamMode mode);
  void shutdownDueToLowBattery();
  void setShiftAlphaStatus(Ion::Events::ShiftAlphaStatus newStatus);
  Shared::PromptController* promptController();
  void redrawWindow();
  void setDFUBetweenEvents(bool active) { m_dfuBetweenEvents = active; }

  // Ion::Storage::StorageDelegate
  bool storageCanChangeForRecordName(
      const Ion::Storage::Record::Name recordName) const override;
  void storageDidChangeForRecord(const Ion::Storage::Record record) override;
  void storageIsFull() override;

#if EPSILON_GETOPT
  void setInitialAppSnapshot(Escher::App::Snapshot* snapshot) {
    m_initialAppSnapshot = snapshot;
  }
#endif

 private:
  Escher::Window* window() override;
  int numberOfContainerTimers() override;
  Escher::Timer* containerTimerAtIndex(int i) override;
  void listenToExternalEvents() override;
  bool processEvent(Ion::Events::Event event);
  void resetShiftAlphaStatus();
  bool updateAlphaLock();
  void handleRunException();
  void openDFU(bool blocking);
  static void ShowCursor();

  static const I18n::Message k_promptMessages[];
  static const KDColor k_promptColors[];
  static const int k_promptNumberOfMessages;
  bool m_firstUSBEnumeration;
  bool m_dfuBetweenEvents;
  AppsWindow m_window;
  EmptyBatteryWindow m_emptyBatteryWindow;
  Shared::GlobalContext m_globalContext;
  ExamPopUpController m_examPopUpController;
  Shared::PromptController m_promptController;
  BatteryTimer m_batteryTimer;
  SuspendTimer m_suspendTimer;
  BacklightDimmingTimer m_backlightDimmingTimer;
  Escher::BlinkTimer m_blinkTimer;
  Home::App::Snapshot m_homeSnapshot;
  OnBoarding::App::Snapshot m_onBoardingSnapshot;
  HardwareTest::App::Snapshot m_hardwareTestSnapshot;
  USB::App::Snapshot m_usbConnectedSnapshot;
#if EPSILON_GETOPT
  // Used to launch a given app on a simulator
  Escher::App::Snapshot* m_initialAppSnapshot;
#endif
};

#endif
