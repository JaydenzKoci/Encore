
#include "enctime.h"

#include <iostream>

SongTime TheSongTime;

void SongTime::SetOffset(double audioCalibration) {
    aCalib = audioCalibration;
};

void SongTime::Reset() {
    pauseTime = 0.0;
    pausedSongPosition = 0.0;
    resumeTargetTime = 0.0;
    running = false;
    paused = false;
    inResumeGracePeriod = false;
}
// start at audio beginning
void SongTime::Start(double end) {
    if (!running) {
        startTime = GetTime() + aCalib;
        endTime = end + aCalib;
        running = true;
        paused = false;
        std::cout << "Started gameplay";
    }
};
double SongTime::GetFakeStartTime() {
    return fakeStartTime;
}

// start at a specific time
void SongTime::Start(double start, double end) {
    if (!running) {
        startTime = GetTime() - start + aCalib;
        fakeStartTime = GetTime() + aCalib;
        endTime = end + aCalib;
        running = true;
        paused = false;
    }
};
void SongTime::Pause() {
    if (running && !paused) {
        pauseTime = GetTime();
        pausedSongPosition = GetSongTime();
        running = false;
        paused = true;
        inResumeGracePeriod = false;
    }
};

void SongTime::Resume() {
    if (!running && paused) {
        double resumeGracePeriod = 3.0;
        double resumeStartPosition = pausedSongPosition - resumeGracePeriod;

        if (resumeStartPosition < 0.0) {
            resumeStartPosition = 0.0;
        }

        startTime = GetTime() - resumeStartPosition + aCalib;
        resumeTargetTime = pausedSongPosition;
        inResumeGracePeriod = true;
        
        pauseTime = 0.0;
        running = true;
        paused = false;
    }
};

void SongTime::Stop() {
    running = false;
    paused = false;
    inResumeGracePeriod = false;
    pausedSongPosition = 0.0;
    resumeTargetTime = 0.0;
}
bool SongTime::Running() {
    return running;
}

double SongTime::GetSongTime() {
    if (!paused && running) {
        double currentTime = GetTime() - startTime;

        if (inResumeGracePeriod && currentTime >= resumeTargetTime) {
            inResumeGracePeriod = false;
        }
        
        return currentTime;
    }
    else if (paused) {
        return pauseTime - startTime;
    }
    return 0.0;
};
double SongTime::GetStartTime() {
    return startTime;
}

// this is actually a lie. it returns "the system time when it ends" i think i forgort
// use GetSongLength if you need song duration
double SongTime::GetEndTime() {
    return endTime + startTime;
}

double SongTime::GetSongLength() {
    return endTime;
}

bool SongTime::SongComplete() {
    if (running) {
        return GetSongTime() > endTime;
    }
    return false;
}

bool SongTime::IsInResumeGracePeriod() {
    return inResumeGracePeriod;
}

double SongTime::GetPausedSongPosition() {
    return pausedSongPosition;
}

double SongTime::GetResumeTargetTime() {
    return resumeTargetTime;
}