
#ifdef	STD_INCLUDE_DECL

#define AUDIO__SAMPLES_PER_SECOND  ( 48000 )
#define AUDIO__BYTES_PER_SAMPLE    ( 2 )
#define AUDIO__CHANNEL_COUNT       ( 2 )
#define AUDIO__BUFFER_SECONDS      ( 2 )
#define AUDIO__WRITE_FRAMES        ( 3 )

struct AUDIO_DATA {
    uint8 * audio;
    uint32 samples;
};

struct AUDIO_ENTRY {
    uint8 * audio;
    int32 samples;
    
    int32 samplesPlayed;
    
    flo32 volume;
};

struct ASSET_SYSTEM;
#define AUDIO__MAX_ENTRY_COUNT  ( 256 )
struct AUDIO_SYSTEM {
    ASSET_SYSTEM * assetSystem;
    
    flo32 mainVolume;
    
    int32     advance_count;
    int32       audio_count;
    AUDIO_ENTRY audio[ AUDIO__MAX_ENTRY_COUNT ];
    
    uint8 * mixBuffer;
    int32   mixBuffer_size;
};

struct MUSIC_SYSTEM {
    ASSET_SYSTEM * assetSystem;
    
    flo32 mainVolume;
    
    uint8 * audio;
    int32 samples;
    
    int32 samplesPlayed;
    
    uint8 * mixBuffer;
    int32   mixBuffer_size;
    
    flo32 volume;
};

#endif	// STD_INCLUDE_DECL
#ifdef	STD_INCLUDE_FUNC

internal AUDIO_DATA
AudioData( void * audio, uint32 samples ) {
    AUDIO_DATA result = { ( uint8 * )audio, samples };
    return result;
}

internal AUDIO_ENTRY
AudioEntry( AUDIO_DATA audio, flo32 volume ) {
    AUDIO_ENTRY result = { audio.audio, (int32)audio.samples, 0, volume };
    return result;
}

internal void
advanceAudio( AUDIO_SYSTEM * audioSystem, uint32 samplesToAdvance ) {
    for( int32 audio_index = 0; audio_index < audioSystem->advance_count; audio_index++ ) {
        audioSystem->audio[ audio_index ].samplesPlayed += samplesToAdvance;
    }
    
    int32 newAudioCount = 0;
    for( int32 audio_index = 0; audio_index < audioSystem->audio_count; audio_index++ ) {
        if( audioSystem->audio[ audio_index ].samplesPlayed < audioSystem->audio[ audio_index ].samples ) {
            audioSystem->audio[ newAudioCount++ ] = audioSystem->audio[ audio_index ];
        }
    }
    audioSystem->audio_count   = newAudioCount;
    audioSystem->advance_count = audioSystem->audio_count;
}

internal void
mixAudio( AUDIO_SYSTEM * audioSystem, uint32 samplesToAdvance ) {
    int32 maxSamplesToWrite = ( AUDIO__SAMPLES_PER_SECOND / 60 ) * AUDIO__WRITE_FRAMES;
    
    for( int32 audio_index = 0; audio_index < audioSystem->audio_count; audio_index++ ) {
        AUDIO_ENTRY * entry = audioSystem->audio + audio_index;
        
        int32 samplesToWrite = entry->samples - entry->samplesPlayed;
        samplesToWrite = minValue( samplesToWrite, maxSamplesToWrite );
        
        uint16 * src  = ( uint16 * )( entry->audio + ( entry->samplesPlayed * AUDIO__BYTES_PER_SAMPLE * AUDIO__CHANNEL_COUNT ) );
        uint16 * dest = ( uint16 * )audioSystem->mixBuffer;
        for( int32 sample_index = 0; sample_index < samplesToWrite; sample_index++ ) {
            int16 value0 = *src++;
            int16 value1 = *src++;
            
            value0 = ( int16 )( ( flo32 )value0 * entry->volume * audioSystem->mainVolume );
            value1 = ( int16 )( ( flo32 )value1 * entry->volume * audioSystem->mainVolume );
            
            *dest++ += value0;
            *dest++ += value1;
        }
    }
}

internal void
playAudio( AUDIO_SYSTEM * audioSystem, AUDIO_DATA audio, flo32 volume ) {
    Assert( audioSystem->audio_count < AUDIO__MAX_ENTRY_COUNT );
    audioSystem->audio[ audioSystem->audio_count++ ] = AudioEntry( audio, volume );
}

//----------
// music functions
//----------

internal void
advanceMusic( MUSIC_SYSTEM * musicSystem, uint32 samplesToAdvance ) {
    musicSystem->samplesPlayed += samplesToAdvance;
    if( musicSystem->samplesPlayed > musicSystem->samples ) {
        musicSystem->samplesPlayed = 0;
    }
}

internal void
mixMusic( MUSIC_SYSTEM * musicSystem, uint32 samplesToAdvance ) {
    int32 maxSamplesToWrite = ( AUDIO__SAMPLES_PER_SECOND / 60 ) * AUDIO__WRITE_FRAMES;
    
    int32 samplesToWrite = musicSystem->samples - musicSystem->samplesPlayed;
    samplesToWrite = minValue( samplesToWrite, maxSamplesToWrite );
    
    uint16 * src  = ( uint16 * )( musicSystem->audio + ( musicSystem->samplesPlayed * AUDIO__BYTES_PER_SAMPLE * AUDIO__CHANNEL_COUNT ) );
    uint16 * dest = ( uint16 * )musicSystem->mixBuffer;
    for( int32 sample_index = 0; sample_index < samplesToWrite; sample_index++ ) {
        int16 value0 = *src++;
        int16 value1 = *src++;
        
        value0 = ( int16 )( ( flo32 )value0 * musicSystem->volume * musicSystem->mainVolume );
        value1 = ( int16 )( ( flo32 )value1 * musicSystem->volume * musicSystem->mainVolume );
        
        *dest++ += value0;
        *dest++ += value1;
    }
}

internal void
playMusic( MUSIC_SYSTEM * musicSystem, AUDIO_DATA music, flo32 volume ) {
    musicSystem->audio   = music.audio;
    musicSystem->samples = music.samples;
    
    musicSystem->samplesPlayed = 0;
    musicSystem->volume        = volume;
}

#endif	// STD_INCLUDE_FUNC