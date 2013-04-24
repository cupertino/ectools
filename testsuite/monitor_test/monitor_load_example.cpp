#include <libec/monitors.h>
#include <libec/process.h>
#include <libec/sensors.h>
// This using namespace is only to keep the example as short as possible
// but in C++ is better to avoid it.
using namespace cea;
class MyMonitor : public Monitor
{
public:
  // Column tags
  static const int NAME = 0;
  static const int PID = 1;
  static const int SENSOR_U64 = 2;
  static const int SENSOR_FLOAT = 3;

  // Member
  bool isFreezed;
  // Constructor
  MyMonitor() :
      isFreezed(false)
  {
    // Add loader
    loadMapMonitorable<FakeSensor>("sensor", "FakeSensor");
  }
  // Event raised by Monitor after a Row created
  void
  onRowCreate(Row& r)
  {
    if (r.tag == FEEDER_PROCESS_ITEM)
      {
        setValue(r, NAME, cast<Process>(r)->getName());
        setValue(r, PID, cast<Process>(r)->getPid());
      }
  }
  // Event raised when a row need updated
  void
  onRowUpdate(Row& r)
  {
    if (r.tag == FEEDER_PROCESS_ITEM)
      {
        Process& p = cast<Process>(r);
        // Update Sensor U64
        for (ColumnIterator<SENSOR_U64, Sensor> s = this; s != 0; ++s)
          {
            setValue(r, s, s->getValue(p.getPid()).U64);
          }
        // Update Sensor Float
        for (ColumnIterator<SENSOR_FLOAT, Sensor> s = this; s != 0; ++s)
          {
            setValue(r, s, s->getValue(p.getPid()).Float);
          }
      }
  }
};
int
main()
{
  // Model
  MyMonitor m;
  ProcessEnumerator pe;
  // Connect Process enumerator to monitor
  m.connectRowFeeder(&pe);
  // Load monitor
  m.loadFile("monitor.xml");

  // Variable to know application running status
  bool isRunning = true;
  // Main Loop
  while (isRunning)
    {
      // Update Monitor
      if (!m.isFreezed)
        {
          m.update();
          pe.update();
        }
      // Handle Terminal Event
      int c = Console::keyHit();
      switch (c)
        {
      case 0:
        // = No key hit (keyHit is async method -> return 0 if no key)
        break;
      case 'q':
      case 'Q':
        // Key for exit the application
        isRunning = false;
        break;
      case 'p':
      case 'P':
        // Key to pause the monitor
        m.isFreezed = !m.isFreezed;
        break;

      case 'r':
        // Refresh
        m.loadFile("monitor.xml");
      default:
        // Create a Terminal KeyHit GridCtrl event
        GridCtrl::Event e(GridCtrl::Event::TERM_KEYHIT, c);

        // If other key -> let the monitor controllers handle the event
        m.handleEvent(e);
        }
      // Render Views
      m.renderViews();
      // Do a sleep of 50 ms to avoid comsuption of all CPU
      // with letting the interpretation of key event
      Tools::sleep_ms(50);
    };
  return 0;
}
