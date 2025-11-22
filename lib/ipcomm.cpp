// This source code file was last time modified by Igor UA3DJY on September 2nd, 2017
// All changes are shown in the patch file coming together with the full atdx source code.

#include <QDebug>
#include <QString>
#include <QSharedMemory>
#include <QSystemSemaphore>

#include "../commons.h"

// Multiple instances: KK1D, 17 Jul 2013
QSharedMemory mem_atdxjt9;

// Semaphore not changed, as the acquire/release calls do not 
// appear to be used anywhere.  
QSystemSemaphore sem_atdxjt9("sem_atdxjt9", 1, QSystemSemaphore::Open);

extern "C" {
  bool attach_atdxjt9_();
  bool create_atdxjt9_(int nsize);
  bool detach_atdxjt9_();
  bool lock_atdxjt9_();
  bool unlock_atdxjt9_();
  struct jt9com * address_atdxjt9_();
  int size_atdxjt9_();
// Multiple instances:  wrapper for QSharedMemory::setKey()
  bool setkey_atdxjt9_(char* mykey, int mykey_len);

  bool acquire_atdxjt9_();
  bool release_atdxjt9_();
}

bool attach_atdxjt9_() {return mem_atdxjt9.attach();}
bool create_atdxjt9_(int nsize) {return mem_atdxjt9.create(nsize);}
bool detach_atdxjt9_() {return mem_atdxjt9.detach();}
bool lock_atdxjt9_() {return mem_atdxjt9.lock();}
bool unlock_atdxjt9_() {return mem_atdxjt9.unlock();}
struct jt9com * address_atdxjt9_() {return reinterpret_cast<struct jt9com *>(mem_atdxjt9.data());}
int size_atdxjt9_() {return (int)mem_atdxjt9.size();}

// Multiple instances:
bool setkey_atdxjt9_(char* mykey, int mykey_len) {
   char *tempstr = (char *)calloc(mykey_len+1,1);
   memset(tempstr, 0, mykey_len+1);
   strncpy(tempstr, mykey, mykey_len);
   QString s1 = QString(QLatin1String(tempstr));
   mem_atdxjt9.setKey(s1);
   return true;}

bool acquire_atdxjt9_() {return sem_atdxjt9.acquire();}
bool release_atdxjt9_() {return sem_atdxjt9.release();}
