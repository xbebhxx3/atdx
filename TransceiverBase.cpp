#include "TransceiverBase.hpp"

#include <exception>

#include <QString>
#include <QTimer>
#include <QThread>
#include <QDebug>

#include "moc_TransceiverBase.cpp"

namespace
{
  auto const unexpected = TransceiverBase::tr ("Unexpected rig error");
}

void TransceiverBase::start (unsigned sequence_number,atdxDateTime * atdxdatetime) noexcept
{
  QString message;
#if atdx_DEBUG_TO_FILE
  FILE * pFile = fopen (debug_file_.c_str(),"a");
  if (atdxtime_ == nullptr)
    fprintf(pFile,"Transceiver start #:%d\n",sequence_number);
  else
    fprintf(pFile,"%s Transceiver start #:%d\n",atdxtime_->currentDateTimeUtc2().toString("hh:mm:ss.zzz").toStdString().c_str(),sequence_number);
  fclose (pFile);
#endif
  try
    {
      last_sequence_number_ = sequence_number;
      atdxtime_ = atdxdatetime;
      may_update u {this, true};
      shutdown ();
#if atdx_DEBUG_TO_FILE
      pFile = fopen (debug_file_.c_str(),"a");
      if (atdxtime_ == nullptr)
        fprintf(pFile,"Transceiver start shutdown done\n");
      else
        fprintf(pFile,"%s Transceiver start shutdown done\n",atdxtime_->currentDateTimeUtc2().toString("hh:mm:ss.zzz").toStdString().c_str());
      fclose (pFile);
#endif
      startup ();
    }
  catch (std::exception const& e)
    {
      message = e.what ();
    }
  catch (...)
    {
      message = unexpected;
    }
  if (!message.isEmpty ())
    {
#if atdx_DEBUG_TO_FILE
      pFile = fopen (debug_file_.c_str(),"a");
      if (atdxtime_ == nullptr)
        fprintf(pFile,"Transceiver start message %s\n",message.toStdString().c_str());
      else
        fprintf(pFile,"%s Transceiver start message %s\n",atdxtime_->currentDateTimeUtc2().toString("hh:mm:ss.zzz").toStdString().c_str(),message.toStdString().c_str());
      fclose (pFile);
#endif
      offline (message);
    }
#if atdx_DEBUG_TO_FILE
  pFile = fopen (debug_file_.c_str(),"a");
  if (atdxtime_ == nullptr)
    fprintf(pFile,"Transceiver start done\n");
  else
    fprintf(pFile,"%s Transceiver start done\n",atdxtime_->currentDateTimeUtc2().toString("hh:mm:ss.zzz").toStdString().c_str());
  fclose (pFile);
#endif
}

void TransceiverBase::set (TransceiverState const& s,
                           unsigned sequence_number) noexcept
{
  TRACE_CAT ("TransceiverBase", "#:" << sequence_number << s);
#if atdx_DEBUG_TO_FILE
  long long ms = 0;
  FILE * pFile = fopen (debug_file_.c_str(),"a");
  if (atdxtime_ == nullptr)
    fprintf(pFile,"Transceiver set state %d #:%d\n",s.online(),sequence_number);
  else {
    fprintf(pFile,"%s Transceiver set state %d #:%d\n",atdxtime_->currentDateTimeUtc2().toString("hh:mm:ss.zzz").toStdString().c_str(),s.online(),sequence_number);
    ms = atdxtime_->currentMSecsSinceEpoch2();
  }
  fclose (pFile);
#endif
  QString message;
  try
    {
      last_sequence_number_ = sequence_number;
      may_update u {this, true};
      bool was_online {requested_.online ()};
      if (!s.online () && was_online)
        {
#if atdx_DEBUG_TO_FILE
          pFile = fopen (debug_file_.c_str(),"a");
          if (atdxtime_ == nullptr)
            fprintf(pFile,"Transceiver shutdown #:%d ",sequence_number);
          else
            fprintf(pFile,"%s Transceiver shutdown #:%d ",atdxtime_->currentDateTimeUtc2().toString("hh:mm:ss.zzz").toStdString().c_str(),sequence_number);
          fclose (pFile);
#endif
          shutdown ();
        }
      else if (s.online () && !was_online)
        {
#if atdx_DEBUG_TO_FILE
          pFile = fopen (debug_file_.c_str(),"a");
          if (atdxtime_ == nullptr)
            fprintf(pFile," restart shutdown #:%d ",sequence_number);
          else
            fprintf(pFile,"%s restart shutdown #:%d ",atdxtime_->currentDateTimeUtc2().toString("hh:mm:ss.zzz").toStdString().c_str(),sequence_number);
          fclose (pFile);
#endif
          shutdown ();
#if atdx_DEBUG_TO_FILE
          pFile = fopen (debug_file_.c_str(),"a");
          if (atdxtime_ == nullptr)
            fprintf(pFile," start #:%d",sequence_number);
          else
            fprintf(pFile,"%s start #:%d",atdxtime_->currentDateTimeUtc2().toString("hh:mm:ss.zzz").toStdString().c_str(),sequence_number);
          fclose (pFile);
#endif
          startup ();
        }
      if (requested_.online ())
        {
          bool audio_cmd {false};
          if (requested_.blocksize() != s.blocksize()) {
#if atdx_DEBUG_TO_FILE
            pFile = fopen (debug_file_.c_str(),"a");
            if (atdxtime_ == nullptr)
              fprintf(pFile,"#:%d blocksize=%d\n",sequence_number,s.blocksize());
            else
              fprintf(pFile,"%s Timing #:%d blocksize=%d\n",atdxtime_->currentDateTimeUtc2().toString("hh:mm:ss.zzz").toStdString().c_str(),sequence_number,s.blocksize());
            fclose (pFile);
#endif
            do_blocksize (s.blocksize());
            audio_cmd = true;
            requested_.blocksize (s.blocksize ());
          }
          if (requested_.period() != s.period()) {
#if atdx_DEBUG_TO_FILE
            pFile = fopen (debug_file_.c_str(),"a");
            if (atdxtime_ == nullptr)
              fprintf(pFile,"#:%d period=%0.1f\n",sequence_number,s.period());
            else
              fprintf(pFile,"%s Timing #:%d period=%0.1f\n",atdxtime_->currentDateTimeUtc2().toString("hh:mm:ss.zzz").toStdString().c_str(),sequence_number,s.period());
            fclose (pFile);
#endif
            do_period (s.period());
            audio_cmd = true;
            requested_.period (s.period ());
          }
          if (requested_.spread() != s.spread()) {
#if atdx_DEBUG_TO_FILE
            pFile = fopen (debug_file_.c_str(),"a");
            if (atdxtime_ == nullptr)
              fprintf(pFile,"#:%d spread=%0.1f\n",sequence_number,s.spread());
            else
              fprintf(pFile,"%s Timing #:%d spread=%0.1f\n",atdxtime_->currentDateTimeUtc2().toString("hh:mm:ss.zzz").toStdString().c_str(),sequence_number,s.spread());
            fclose (pFile);
#endif
            do_spread (s.spread());
            audio_cmd = true;
            requested_.spread (s.spread ());
          }
          if (requested_.nsym() != s.nsym()) {
#if atdx_DEBUG_TO_FILE
            pFile = fopen (debug_file_.c_str(),"a");
            if (atdxtime_ == nullptr)
              fprintf(pFile,"#:%d nsym=%d\n",sequence_number,s.nsym());
            else
              fprintf(pFile,"%s Timing #:%d nsym=%d\n",atdxtime_->currentDateTimeUtc2().toString("hh:mm:ss.zzz").toStdString().c_str(),sequence_number,s.nsym());
            fclose (pFile);
#endif
            do_nsym (s.nsym());
            audio_cmd = true;
            requested_.nsym (s.nsym ());
          }
          if (requested_.trfrequency() != s.trfrequency()) {
#if atdx_DEBUG_TO_FILE
            pFile = fopen (debug_file_.c_str(),"a");
            if (atdxtime_ == nullptr)
              fprintf(pFile,"#:%d trfrequency=%0.1f\n",sequence_number,s.trfrequency());
            else
              fprintf(pFile,"%s Timing #:%d trfrequency=%0.1f\n",atdxtime_->currentDateTimeUtc2().toString("hh:mm:ss.zzz").toStdString().c_str(),sequence_number,s.trfrequency());
            fclose (pFile);
#endif
            do_trfrequency (s.trfrequency());
            audio_cmd = true;
            requested_.trfrequency (s.trfrequency ());
          }
          if (requested_.volume() != s.volume()) {
#if atdx_DEBUG_TO_FILE
            pFile = fopen (debug_file_.c_str(),"a");
            if (atdxtime_ == nullptr)
              fprintf(pFile,"#:%d volume=%0.1f\n",sequence_number,s.volume());
            else
              fprintf(pFile,"%s Timing #:%d volume=%0.1f\n",atdxtime_->currentDateTimeUtc2().toString("hh:mm:ss.zzz").toStdString().c_str(),sequence_number,s.volume());
            fclose (pFile);
#endif
            do_txvolume (s.volume());
            audio_cmd = true;
            requested_.volume (s.volume ());
          }
          if (requested_.audio() != s.audio()) {
#if atdx_DEBUG_TO_FILE
            pFile = fopen (debug_file_.c_str(),"a");
            if (atdxtime_ == nullptr)
              fprintf(pFile,"#:%d audio=%d\n",sequence_number,s.audio());
            else
              fprintf(pFile,"%s Timing #:%d audio=%d\n",atdxtime_->currentDateTimeUtc2().toString("hh:mm:ss.zzz").toStdString().c_str(),sequence_number,s.audio());
            fclose (pFile);
#endif
            do_audio (s.audio());
            audio_cmd = true;
            requested_.audio (s.audio ());
          }
          if (requested_.tx_audio() != s.tx_audio()) {
#if atdx_DEBUG_TO_FILE
            pFile = fopen (debug_file_.c_str(),"a");
            if (atdxtime_ == nullptr)
              fprintf(pFile,"#:%d tx_audio=%d\n",sequence_number,s.tx_audio());
            else
              fprintf(pFile,"%s Timing #:%d tx_audio=%d\n",atdxtime_->currentDateTimeUtc2().toString("hh:mm:ss.zzz").toStdString().c_str(),sequence_number,s.tx_audio());
            fclose (pFile);
#endif
            if (s.tx_audio()) {
              do_modulator_start(s.symbolslength (), s.framespersymbol (), s.trfrequency (), s.tonespacing (), s.synchronize (),s.dbsnr (), s.trperiod ());
              requested_.symbolslength(s.symbolslength ());
              requested_.framespersymbol(s.framespersymbol ());
              requested_.trfrequency(s.trfrequency ());
              requested_.tonespacing(s.tonespacing ());
              requested_.synchronize(s.synchronize ());
              requested_.dbsnr(s.dbsnr ());
              requested_.trperiod(s.trperiod ());
            } else {
              do_modulator_stop(s.quick ());
              requested_.quick (s.quick ());
            }
            audio_cmd = true;

            requested_.tx_audio (s.tx_audio ());
          }


          if (requested_.tune() != s.tune()) {
#if atdx_DEBUG_TO_FILE
            pFile = fopen (debug_file_.c_str(),"a");
            if (atdxtime_ == nullptr)
              fprintf(pFile,"#:%d tune=%d\n",sequence_number,s.tune());
            else
              fprintf(pFile,"%s Timing #:%d tune=%d\n",atdxtime_->currentDateTimeUtc2().toString("hh:mm:ss.zzz").toStdString().c_str(),sequence_number,s.tune());
            fclose (pFile);
#endif
            do_tune (s.tune());
            audio_cmd = true;
            requested_.tune (s.tune ());
          }
          if (!audio_cmd) {
            bool ptt_on {false};
            bool ptt_off {false};
            if (requested_.ft4_mode() != s.ft4_mode()) {
  #if atdx_DEBUG_TO_FILE
              pFile = fopen (debug_file_.c_str(),"a");
              if (atdxtime_ == nullptr)
                fprintf(pFile,"#:%d ft4_mode=%d\n",sequence_number,s.ft4_mode());
              else
                fprintf(pFile,"%s Timing #:%d ft4_mode=%d\n",atdxtime_->currentDateTimeUtc2().toString("hh:mm:ss.zzz").toStdString().c_str(),sequence_number,s.ft4_mode());
              fclose (pFile);
  #endif
              do_post_ft4_mode (s.ft4_mode());
              requested_.ft4_mode (s.ft4_mode ());
            }
            if (s.ptt () != requested_.ptt ())
              {
                ptt_on = s.ptt ();
                ptt_off = !s.ptt ();
              }
            if (ptt_off)
              {
  #if atdx_DEBUG_TO_FILE
                pFile = fopen (debug_file_.c_str(),"a");
                if (atdxtime_ == nullptr)
                  fprintf(pFile,"#:%d ptt_off\n",sequence_number);
                else
                  fprintf(pFile,"%s Timing #:%d ptt_off\n",atdxtime_->currentDateTimeUtc2().toString("hh:mm:ss.zzz").toStdString().c_str(),sequence_number);
                fclose (pFile);
  #endif
                do_ptt (false);
                do_post_ptt (false);
                if (!requested_.audio()) QThread::msleep (100); // some rigs cannot process CAT
                                       // commands while switching from
                                       // Tx to Rx
              }
            if (s.frequency ()    // ignore bogus zero frequencies
                && ((s.frequency () != requested_.frequency () // and QSY
                     || (s.mode () != UNK && s.mode () != requested_.mode ())) // or mode change
                    || ptt_off))       // or just returned to rx
              {
  #if atdx_DEBUG_TO_FILE
                pFile = fopen (debug_file_.c_str(),"a");
                if (atdxtime_ == nullptr)
                  fprintf(pFile,"#:%d do_frequency %lld\n",sequence_number,s.frequency ());
                else
                  fprintf(pFile,"%s Timing #:%d do_frequency %lld\n",atdxtime_->currentDateTimeUtc2().toString("hh:mm:ss.zzz").toStdString().c_str(),sequence_number,s.frequency ());
                fclose (pFile);
  #endif
                requested_.frequency (s.frequency ());
                requested_.mode (s.mode ());
                do_frequency (s.frequency (), s.mode (), ptt_off);
                do_post_frequency (s.frequency (), s.mode ());

                // record what actually changed
                requested_.frequency (actual_.frequency ());
                requested_.mode (actual_.mode ());
              }
            else if (!s.tx_frequency ()
                || (s.tx_frequency () > 10000 // ignore bogus startup values
                    && s.tx_frequency () < std::numeric_limits<Frequency>::max () - 10000))
              {
                if ((s.tx_frequency () != requested_.tx_frequency () // and QSY
                     || (s.mode () != UNK && s.mode () != requested_.mode ())) // or mode change
                    // || s.split () != requested_.split ())) // or split change
                    || (s.tx_frequency () && ptt_on)) // or about to tx split
                  {
  #if atdx_DEBUG_TO_FILE
                    pFile = fopen (debug_file_.c_str(),"a");
                    if (atdxtime_ == nullptr)
                      fprintf(pFile,"#:%d do_tx_frequency %lld\n",sequence_number,s.tx_frequency ());
                    else
                      fprintf(pFile,"%s Timing #:%d do_tx_frequency %lld\n",atdxtime_->currentDateTimeUtc2().toString("hh:mm:ss.zzz").toStdString().c_str(),sequence_number,s.tx_frequency ());
                    fclose (pFile);
  #endif
                    requested_.tx_frequency (s.tx_frequency ());
                    requested_.split (s.tx_frequency () != 0);
                    do_tx_frequency (s.tx_frequency (), s.mode (), ptt_on);
                    do_post_tx_frequency (s.tx_frequency (), s.mode ());

                    // record what actually changed
                    requested_.tx_frequency (actual_.tx_frequency ());
                    requested_.split (actual_.split ());
                  }
              }
            if (ptt_on)
              {
  #if atdx_DEBUG_TO_FILE
                pFile = fopen (debug_file_.c_str(),"a");
                if (atdxtime_ == nullptr)
                  fprintf(pFile,"#:%d ptt_on\n",sequence_number);
                else
                  fprintf(pFile,"%s Timing #:%d ptt_on\n",atdxtime_->currentDateTimeUtc2().toString("hh:mm:ss.zzz").toStdString().c_str(),sequence_number);
                fclose (pFile);
  #endif
                do_ptt (true);
                do_post_ptt (true);
  //              QThread::msleep (100 + ms); // some rigs cannot process CAT
                                       // commands while switching from
                                       // Rx to Tx
              }

            // record what actually changed
            if (ptt_on || ptt_off) requested_.ptt (actual_.ptt ());
          }
        } else {
#if atdx_DEBUG_TO_FILE
          pFile = fopen (debug_file_.c_str(),"a");
          if (atdxtime_ == nullptr)
            fprintf (pFile,"#:%d NOT ONLINE\n",sequence_number);
          else
            fprintf (pFile,"%s #:%d NOT ONLINE\n",atdxtime_->currentDateTimeUtc2().toString("hh:mm:ss.zzz").toStdString().c_str(),sequence_number);
          fclose (pFile);
#endif
        }
    }
  catch (std::exception const& e)
    {
      message = e.what ();
    }
  catch (...)
    {
      message = unexpected;
    }
  
  if (!message.isEmpty ())
    {
#if atdx_DEBUG_TO_FILE
      pFile = fopen (debug_file_.c_str(),"a");
      if (atdxtime_ == nullptr)
        fprintf(pFile,"#:%d set message %s\n",sequence_number,message.toStdString().c_str());
      else
        fprintf(pFile,"%s #:%d set message %s\n",atdxtime_->currentDateTimeUtc2().toString("hh:mm:ss.zzz").toStdString().c_str(),sequence_number,message.toStdString().c_str());
      fclose (pFile);
#endif
      offline (message);
    }
#if atdx_DEBUG_TO_FILE
      pFile = fopen (debug_file_.c_str(),"a");
      if (atdxtime_ == nullptr)
        fprintf(pFile,"Transceiver set %d #:%d end\n",s.online(),sequence_number);
      else
        fprintf(pFile,"%s Transceiver set %d #:%d end %lld ms.\n",atdxtime_->currentDateTimeUtc2().toString("hh:mm:ss.zzz").toStdString().c_str(),s.online(),sequence_number,atdxtime_->currentMSecsSinceEpoch2() - ms);
      fclose (pFile);
#endif
}

void TransceiverBase::startup ()
{
  QString message;
#if atdx_DEBUG_TO_FILE
  long long ms = 0;
  FILE * pFile = fopen (debug_file_.c_str(),"a");
  if (atdxtime_ == nullptr)
    fprintf(pFile,"Transceiver startup\n");
  else {
    fprintf(pFile,"%s Transceiver startup\n",atdxtime_->currentDateTimeUtc2().toString("hh:mm:ss.zzz").toStdString().c_str());
    ms = atdxtime_->currentMSecsSinceEpoch2();
  }
  fclose (pFile);
#endif
  try
    {
      actual_.online (true);
      requested_.online (true);
      auto res = do_start (atdxtime_);
#if atdx_DEBUG_TO_FILE
      pFile = fopen (debug_file_.c_str(),"a");
      if (atdxtime_ == nullptr)
        fprintf(pFile,"do_start\n");
      else
        fprintf(pFile,"%s Timing do_start\n",atdxtime_->currentDateTimeUtc2().toString("hh:mm:ss.zzz").toStdString().c_str());
      fclose (pFile);
#endif
      do_post_start (atdxtime_);
      Q_EMIT resolution (res);
    }
  catch (std::exception const& e)
    {
      message = e.what ();
    }
  catch (...)
    {
      message = unexpected;
    }
  if (!message.isEmpty ())
    {
#if atdx_DEBUG_TO_FILE
      pFile = fopen (debug_file_.c_str(),"a");
      if (atdxtime_ == nullptr)
        fprintf(pFile,"Transceiver startup message %s\n",message.toStdString().c_str());
      else
        fprintf(pFile,"%s Transceiver startup message %s\n",atdxtime_->currentDateTimeUtc2().toString("hh:mm:ss.zzz").toStdString().c_str(),message.toStdString().c_str());
      fclose (pFile);
#endif
      offline (message);
    }
#if atdx_DEBUG_TO_FILE
      pFile = fopen (debug_file_.c_str(),"a");
      if (atdxtime_ == nullptr)
        fprintf(pFile,"Transceiver startup end\n");
      else
        fprintf(pFile,"%s Transceiver startup end %lld ms.\n",atdxtime_->currentDateTimeUtc2().toString("hh:mm:ss.zzz").toStdString().c_str(),atdxtime_->currentMSecsSinceEpoch2() - ms);
      fclose (pFile);
#endif
}

void TransceiverBase::shutdown ()
{
#if atdx_DEBUG_TO_FILE
  long long ms = 0;
  FILE * pFile = fopen (debug_file_.c_str(),"a");
  if (atdxtime_ == nullptr) {
    fprintf(pFile,"Transceiver shutdown\n");
  }
  else {
    fprintf(pFile,"%s Transceiver shutdown\n",atdxtime_->currentDateTimeUtc2().toString("hh:mm:ss.zzz").toStdString().c_str());
    ms = atdxtime_->currentMSecsSinceEpoch2();
  }
  fclose (pFile);
#endif
  may_update u {this};
#if atdx_DEBUG_TO_FILE
  pFile = fopen (debug_file_.c_str(),"a");
  if (atdxtime_ == nullptr)
    fprintf(pFile,"may_update\n");
  else
    fprintf(pFile,"%s Timing may_update\n",atdxtime_->currentDateTimeUtc2().toString("hh:mm:ss.zzz").toStdString().c_str());
  fclose (pFile);
#endif
  if (requested_.online ())
    {
      try
        {
          // try and ensure PTT isn't left set
          do_ptt (false);
#if atdx_DEBUG_TO_FILE
          pFile = fopen (debug_file_.c_str(),"a");
          if (atdxtime_ == nullptr)
            fprintf(pFile,"do_ptt:0\n");
          else
            fprintf(pFile,"%s Timing do_ptt:0\n",atdxtime_->currentDateTimeUtc2().toString("hh:mm:ss.zzz").toStdString().c_str());
          fclose (pFile);
#endif
          do_post_ptt (false);
          if (requested_.split ())
            {
              // try and reset split mode
              do_tx_frequency (0, UNK, true);
#if atdx_DEBUG_TO_FILE
            pFile = fopen (debug_file_.c_str(),"a");
            if (atdxtime_ == nullptr)
              fprintf(pFile,"do_tx_freq:0\n");
            else
              fprintf(pFile,"%s Timing do_tx_freq:0\n",atdxtime_->currentDateTimeUtc2().toString("hh:mm:ss.zzz").toStdString().c_str());
            fclose (pFile);
#endif
              do_post_tx_frequency (0, UNK);
            }
        }
      catch (...)
        {
          // don't care about exceptions
        }
    }
//  printf("Transceiver shutdow b4 dostop()\n");
  do_stop ();
#if atdx_DEBUG_TO_FILE
  pFile = fopen (debug_file_.c_str(),"a");
  if (atdxtime_ == nullptr)
    fprintf(pFile,"do_stop\n");
  else
    fprintf(pFile,"%s Timing do_stop\n",atdxtime_->currentDateTimeUtc2().toString("hh:mm:ss.zzz").toStdString().c_str());
  fclose (pFile);
#endif
  do_post_stop ();
  actual_ = TransceiverState {};
  requested_ = TransceiverState {};
#if atdx_DEBUG_TO_FILE
  pFile = fopen (debug_file_.c_str(),"a");
  if (atdxtime_ == nullptr)
    fprintf(pFile,"Transceiver shutdown end\n");
  else
    fprintf(pFile,"%s Transceiver shutdown end %lld ms.\n",atdxtime_->currentDateTimeUtc2().toString("hh:mm:ss.zzz").toStdString().c_str(),atdxtime_->currentMSecsSinceEpoch2() - ms);
  fclose (pFile);
#endif
//  atdxtime_ = nullptr;
}

void TransceiverBase::stop () noexcept
{
  QString message;
#if atdx_DEBUG_TO_FILE
  FILE * pFile = fopen (debug_file_.c_str(),"a");
  if (atdxtime_ == nullptr)
    fprintf(pFile,"Transceiver stop\n");
  else
    fprintf(pFile,"%s Transceiver stop\n",atdxtime_->currentDateTimeUtc2().toString("hh:mm:ss.zzz").toStdString().c_str());
  fclose (pFile);
#endif
  try
    {
      shutdown ();
    }
  catch (std::exception const& e)
    {
      message = e.what ();
    }
  catch (...)
    {
      message = unexpected;
    }
  if (!message.isEmpty ())
    {
      offline (message);
    }
  else
    {
      Q_EMIT finished ();
    }
#if atdx_DEBUG_TO_FILE
  pFile = fopen (debug_file_.c_str(),"a");
  if (atdxtime_ == nullptr)
    fprintf(pFile,"Transceiver stop end\n");
  else
    fprintf(pFile,"%s Transceiver stop end\n",atdxtime_->currentDateTimeUtc2().toString("hh:mm:ss.zzz").toStdString().c_str());
  fclose (pFile);
#endif
}

void TransceiverBase::update_rx_frequency (Frequency rx)
{
  if (rx)
    {
      actual_.frequency (rx);
      requested_.frequency (rx);    // track rig changes
    }
}

void TransceiverBase::update_other_frequency (Frequency tx)
{
  actual_.tx_frequency (tx);
}

void TransceiverBase::update_split (bool state)
{
  actual_.split (state);
}

void TransceiverBase::update_mode (MODE m)
{
  actual_.mode (m);
  requested_.mode (m);    // track rig changes
}

void TransceiverBase::update_PTT (bool state)
{
  actual_.ptt (state);
}

void TransceiverBase::update_level (int l)
{
  actual_.level (l);
//  requested_.level (l);    // track rig changes
}

void TransceiverBase::update_power (unsigned int p)
{
  actual_.power (p);
//  requested_.power (p);    // track rig changes
}

void TransceiverBase::update_swr (unsigned int p)
{
  actual_.swr (p);
//  requested_.swr (p);    // track rig changes
}

void TransceiverBase::update_complete (bool force_signal)
{
  if ((do_pre_update () && actual_ != last_) || force_signal)
    {
      Q_EMIT update (actual_, last_sequence_number_);
      last_ = actual_;
    }
}

void TransceiverBase::offline (QString const& reason)
{
#if atdx_DEBUG_TO_FILE
  FILE * pFile = fopen (debug_file_.c_str(),"a");
  if (atdxtime_ == nullptr)
    fprintf(pFile,"Transceiver offline %s\n",reason.toStdString().c_str());
  else
    fprintf(pFile,"%s Transceiver offline %s\n",atdxtime_->currentDateTimeUtc2().toString("hh:mm:ss.zzz").toStdString().c_str(),reason.toStdString().c_str());
  fclose (pFile);
#endif
  Q_EMIT failure (reason);
  try
    {
      shutdown ();
    }
  catch (...)
    {
      // don't care
    }
}
