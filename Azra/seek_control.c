void seek_forward()
{
	pthread_cancel(sthreadid.playbacktid);
	pthread_cancel(sthreadid.timertid);
	snd_pcm_drop(pcm_handle);
	snd_pcm_prepare(pcm_handle);
	seconds += 10;
	lseek64(sfiledesc.ifiledesc, swavheaderinfo.ibyterate*(seconds) , SEEK_SET);
	pthread_create(&playbacktid, NULL,cml_audioplayer_play, NULL);
	pthread_create(&timertid, NULL, cml_timer_display, NULL);
}

void seek_backward()
{
	pthread_cancel(sthreadid.playbacktid);
	pthread_cancel(sthreadid.timertid);
	snd_pcm_drop(pcm_handle);
	snd_pcm_prepare(pcm_handle);
	seconds -= 10;
	lseek64(sfiledesc.ifiledesc, swavheaderinfo.ibyterate*(seconds) , SEEK_SET);
	pthread_create(&playbacktid, NULL,cml_audioplayer_play, NULL);
	pthread_create(&timertid, NULL, cml_timer_display, NULL);
}

void seek_to(int s)
{
	pthread_cancel(sthreadid.playbacktid);
	pthread_cancel(sthreadid.timertid);
	snd_pcm_drop(pcm_handle);
	snd_pcm_prepare(pcm_handle);
	seconds = s;
	lseek64(sfiledesc.ifiledesc, swavheaderinfo.ibyterate*(seconds) , SEEK_SET);
	pthread_create(&playbacktid, NULL,cml_audioplayer_play, NULL);
	pthread_create(&timertid, NULL, cml_timer_display, NULL);
}
