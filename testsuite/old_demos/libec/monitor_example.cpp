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

  // Constructor
  MyMonitor()
  {
    addColumn("Name", Value::STRING, NAME, 30, 30);
    addColumn("PID", Value::INT, PID);
  }

  // Add Sensor column
  void
  addSensor(Sensor* s)
  {
    if (s->getType() == U64)
      {
        addMappedColumn(s, s->getAlias(), Value::INT, SENSOR_U64);
      }
    else
      {
        addMappedColumn(s, s->getAlias(), Value::DOUBLE, SENSOR_FLOAT);
      }
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

  // View
  TermGridView view(m);

  // Controller
  TermGridCtrl termCtrl(view);

  // Start Text
  Console::drawText("My Monitor", 0, 0);
  Console::drawText(
      "\t(use '1'..'9' to sort, '='+'1'..'9' to sum, \
'f'+'1'..'9' to fix)", 0,
      1);

  // Place the view
  view.move(0, 3);
  // Set auto size with offset
  view.setAutoSize(GridView::AutoSize(0, 3));

  // Create a style for Sensor
  GridStyle s;
  s.setSuffix(" u/s").setAlign(GridStyle::RIGHT);

  // Set style for sensor Column in the view
  view.setStyle(MyMonitor::SENSOR_U64, s);
  view.setStyle(MyMonitor::SENSOR_FLOAT, s);

  // Add a Fake Sensor
  FakeSensor s1;
  m.addSensor(&s1);

  // Variable to pause monitor and know application running status
  bool isRunning = true, isFreezed = false;

  // Main Loop
  while (isRunning)
    {

      // Update Monitor
      if (!isFreezed)
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
        isFreezed = (!isFreezed);
        // Force render will make needRender return true (see bellow)
        view.forceRender();
        break;
      default:
        // If other key -> let the terminal controller handle the event
        termCtrl.handleTerminalEvent(c);
        }

      // Render
      if (view.needRender())
        {

          // Draw a tips text
          Console::drawText(
              "Columns count=" + Tools::CStr(view.getColumnViewCount())
                  + ", Row count=" + Tools::CStr(view.getRowViewCount())
                  + (isFreezed ? " - PAUSED" : "         "), 0, 2,
              Console::DrawStyle(Color::Yellow));

          // Refresh size of view
          view.refreshAutoSize();
          // Finally render the view
          view.render();
        }

      // Do a sleep of 50 ms to avoid comsuption of all CPU
      // with letting the interpretation of key event
      Tools::sleep_ms(50);

    };

  return 0;
}
