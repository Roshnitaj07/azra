#include<stdio.h>
#include"CML_types.h"
//#include"CML_AudioPlayer_types.h"
#include<alsa/asoundlib.h>
int main(int argc,char **argv)
{
    //cml_myplaylist(argc,argv);
    //sCurFile.icurfileindex=0;
    long loops;
  int rc;
  int size;
  
  unsigned int val;
  int dir;
  //snd_pcm_uframes_t frames;
  //char *buffer;
  
  /* Open PCM device for playback. */
  rc = snd_pcm_open(&salsaconfig.pcm_handle, "default", 
                    SND_PCM_STREAM_PLAYBACK, 0);
  if (rc < 0) {
    fprintf(stderr, 
            "unable to open pcm device: %s\n",
            snd_strerror(rc));
    exit(1);
  }
  
  /* Allocate a hardware parameters object. */
  snd_pcm_hw_params_alloca(&salsaconfig.params);

  /* Fill it in with default values. */
  snd_pcm_hw_params_any(salsaconfig.pcm_handle, salsaconfig.params);

  /* Set the desired hardware parameters. */

  /* Interleaved mode */
  snd_pcm_hw_params_set_access(salsaconfig.pcm_handle, salsaconfig.params,
                      SND_PCM_ACCESS_RW_INTERLEAVED);

  /* Signed 16-bit little-endian format */
  snd_pcm_hw_params_set_format(salsaconfig.pcm_handle, salsaconfig.params,
                              SND_PCM_FORMAT_S16_LE);

  /* Two channels (stereo) */
  snd_pcm_hw_params_set_channels(salsaconfig.pcm_handle, salsaconfig.params, 2);

  /* 44100 bits/second sampling rate (CD quality) */
  val = 44100;
  snd_pcm_hw_params_set_rate_near(salsaconfig.pcm_handle, salsaconfig.params, 
                                  &val, &dir);

  /* Set period size to 32 frames. */
  salsaconfig.frames = 32;
  snd_pcm_hw_params_set_period_size_near(salsaconfig.pcm_handle,
                              salsaconfig.params, &salsaconfig.frames, &dir);

  /* Write the parameters to the driver */
  rc = snd_pcm_hw_params(salsaconfig.pcm_handle, salsaconfig.params);
  if (rc < 0) {
    fprintf(stderr, 
            "unable to set hw parameters: %s\n",
            snd_strerror(rc));
    exit(1);
  }

  /* Use a buffer large enough to hold one period */
  snd_pcm_hw_params_get_period_size(salsaconfig.params, &salsaconfig.frames,
                                    &dir);
  size = salsaconfig.frames * 4; /* 2 bytes/sample, 2 channels */
  salsaconfig.buffer = (char *) malloc(size);

  /* We want to loop for 5 seconds */
  snd_pcm_hw_params_get_period_time(salsaconfig.params, 
                                    &val, &dir);
  /* 5 seconds in microseconds divided by 
   * period time */
  loops = 15000000 / val;

  while (loops > 0) {
    loops--;
    rc = read(0, salsaconfig.buffer, size);
    if (rc == 0) {
      fprintf(stderr, "end of file on input\n");
      break;
    } else if (rc != size) {
      fprintf(stderr,
              "short read: read %d bytes\n", rc);
    }
    rc = snd_pcm_writei(salsaconfig.pcm_handle, salsaconfig.buffer, salsaconfig.frames);
    if (rc == -EPIPE) {
      /* EPIPE means underrun */
      fprintf(stderr, "underrun occurred\n");
      snd_pcm_prepare(salsaconfig.pcm_handle);
    } else if (rc < 0) {
      fprintf(stderr,
              "error from writei: %s\n",
              snd_strerror(rc));
    }  else if (rc != (int)salsaconfig.frames) {
      fprintf(stderr, 
              "short write, write %d frames\n", rc);
    }
  }

  snd_pcm_drain(salsaconfig.pcm_handle);
  snd_pcm_close(salsaconfig.pcm_handle);
  free(salsaconfig.buffer);
  
  return 0;


}
