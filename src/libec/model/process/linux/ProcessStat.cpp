#include <libec/process/linux/ProcessStat.h>

namespace cea
{

  /** +get */
  bool
  ProcessStat::get(pid_t pid, ProcessStat::Field stat, char& c,
      char defaultValue)
  {
    Value v = getValue(pid, stat);
    if (v.type == CHAR)
      {
        c = v.v.c;
        return true;
      }
    else
      {
        c = defaultValue;
        return false;
      }
  }
  bool
  ProcessStat::get(pid_t pid, ProcessStat::Field stat, int& d, int defaultValue)
  {
    Value v = getValue(pid, stat);
    if (v.type == INT)
      {
        d = v.v.d;
        return true;
      }
    else
      {
        d = defaultValue;
        return false;
      }
  }
  bool
  ProcessStat::get(pid_t pid, ProcessStat::Field stat, unsigned int& u,
      unsigned int defaultValue)
  {
    Value v = getValue(pid, stat);
    if (v.type == UINT)
      {
        u = v.v.u;
        return true;
      }
    else
      {
        u = defaultValue;
        return false;
      }
  }
  bool
  ProcessStat::get(pid_t pid, ProcessStat::Field stat, long int& ld,
      long int defaultValue)
  {
    Value v = getValue(pid, stat);
    if (v.type == LINT)
      {
        ld = v.v.ld;
        return true;
      }
    else
      {
        ld = defaultValue;
        return false;
      }
  }
  bool
  ProcessStat::get(pid_t pid, ProcessStat::Field stat, long unsigned int& lu,
      long unsigned int defaultValue)
  {
    Value v = getValue(pid, stat);
    if (v.type == LUINT)
      {
        lu = v.v.lu;
        return true;
      }
    else
      {
        lu = defaultValue;
        return false;
      }
  }
  bool
  ProcessStat::get(pid_t pid, ProcessStat::Field stat,
      long long unsigned int& llu, long long unsigned int defaultValue)
  {
    Value v = getValue(pid, stat);
    if (v.type == LLUINT)
      {
        llu = v.v.llu;
        return true;
      }
    else
      {
        llu = defaultValue;
        return false;
      }
  }
  bool
  ProcessStat::get(pid_t pid, ProcessStat::Field stat, std::string& s,
      const std::string& defaultValue)
  {
    if (getStrValue(pid, stat, s))
      {
        return true;
      }
    else
      {
        s.assign(defaultValue);
        return false;
      }
  }

  /** Get proc/[pid]/stat values */
  ProcessStat::Value
  ProcessStat::getValue(pid_t pid, ProcessStat::Field stat)
  {
    /* Variable for Optimization */
    static Type fieldType[] =
      {
      /*PID*/INT,/*COMM*/STR,/*STATE*/CHAR,/*PPID*/INT,/*PGRP*/INT,
      /*SESSION*/INT,/*TTY_NR*/INT,/*TPGID*/INT,/*FLAGS*/UINT,
      /*MINFLT*/LUINT,/*CMINFLT*/LUINT,/*MAJFLT*/LUINT,/*CMAJFLT*/LUINT,
      /*UTIME*/LUINT,/*STIME*/LUINT,/*CUTIME*/LINT,/*CSTIME*/LINT,
      /*PRIORITY*/LINT,/*NICE*/LINT,/*NUM_THREADS*/LINT,
      /*ITREALVALUE*/LINT,/*STARTTIME*/LLUINT,/*VSIZE*/LUINT,/*RSS*/LINT,
      /*RSSLIM*/LUINT,/*STARTCODE*/LUINT,/*ENDCODE*/LUINT,
      /*STARTSTACK*/LUINT,/*KSTKESP*/LUINT,/*KSTKEIP*/LUINT,
      /*SIGNAL*/LUINT,/*BLOCKED*/LUINT,/*SIGIGNORE*/LUINT,
      /*SIGCATCH*/LUINT,/*WCHAN*/LUINT,/*NSWAP*/LUINT,/*CNSWAP*/LUINT,
      /*EXIT_SIGNAL*/INT,/*PROCESSOR*/INT,/*RT_PRIORITY*/UINT,
      /*POLICY*/UINT,/*DELAYACCT_BLKIO_TICKS*/LLUINT,/*GUEST_TIME*/LUINT,
      /*CGUEST_TIME*/LINT };
    /* Get the String value */
    std::string str = getStr(pid, stat);
    /* Initialize the value variable */
    Value value;
    value.type = UNKNOWN;
    /* Avoid to exceed the fieldType table */
    unsigned int fieldId = (unsigned int) (stat);
    if (fieldId >= sizeof(fieldType) / sizeof(Type))
      return value;
    /* Get the correct type value */
    value.type = fieldType[fieldId];
    switch (fieldType[fieldId])
      {
    case CHAR:
      Tools::CStrTo(str, value.v.c);
      break;
    case INT:
      Tools::CStrTo(str, value.v.d);
      break;
    case UINT:
      Tools::CStrTo(str, value.v.u);
      break;
    case LINT:
      Tools::CStrTo(str, value.v.ld);
      break;
    case LUINT:
      Tools::CStrTo(str, value.v.lu);
      break;
    case LLUINT:
      Tools::CStrTo(str, value.v.llu);
      break;
    default:
      break;
      };
    /* Return the value */
    return value;
  }

  /** +getStr */
  std::string
  ProcessStat::getStr(pid_t pid, ProcessStat::Field stat)
  {
    std::string result;
    /* Get the str value */
    getStrValue(pid, stat, result);
    /* Return */
    return result;
  }

  /** #getStrValue */
  bool
  ProcessStat::getStrValue(pid_t pid, ProcessStat::Field stat,
      std::string& retString)
  {
    /* Open the cmdline file */
    std::string file = "/proc/" + Tools::CStr(pid) + "/stat";
    std::ifstream f(file.c_str());
    std::string data;

    /* If Opened extract Path */
    if (f.is_open())
      {
        /* Get the first line */
        if (f.good())
          std::getline(f, data);
        /* Close the file */
        f.close();
      }

    /* Set the variable */
    unsigned int max = data.length();
    int fieldId = (int) (ProcStatFieldCount) - (int) (stat);

    /* Search the  value */
    if (stat == PID)
      {
        /* Found the first ' ' */
        unsigned int end = 0;
        for (; end < max; end++)
          {
            if (data[end] == ' ')
              break;
          }
        /* Return the value */
        retString.assign(data, 0, end);
        return true;
      }
    else if (stat == COMM)
      {
        /* Get the name */
        unsigned int start = 0;
        /* Found the first '(' */
        for (; start < max; start++)
          {
            if (data[start] == '(')
              {
                start++;
                break;
              }
          }
        /* Found the last ')' */
        for (; max > start; max--)
          {
            if (data[max] == ')')
              break;
          }
        /* Return the name*/
        retString.assign(data, start, max - start);
        return true;
      }
    else
      {
        /* Get the field after COMM field */
        unsigned int oldTok = max, tok = max;
        for (; tok > 0; tok--)
          {
            switch (data[tok])
              {
            case ')': /* reach the end of research */
              if (fieldId == 1)
                {
                  /* Field found */
                  retString.assign(data, tok + 1, oldTok - tok);
                  return true;
                }
              else
                {
                  return false;
                }
              break;
            case ' ': /* new token */
              fieldId--;
              if (fieldId == 0)
                {
                  /* Field found */
                  retString.assign(data, tok + 1, oldTok - tok);
                  return true;
                }
              else
                {
                  /* Advance the token */
                  oldTok = tok - 1;
                }
              break;
              }
          }
      }
    /* Not found by default */
    return false;
  }

}
