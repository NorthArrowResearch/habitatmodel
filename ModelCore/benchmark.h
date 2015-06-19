#ifndef BENCHMARK_H
#define BENCHMARK_H

#include <QTime>
#include <QDebug>

namespace HabitatModel {

class ProcessTimer {

public:
    inline ProcessTimer(QString sProcName){
#ifdef QT_DEBUG
        // We want to measure how long things take
        m_sProcessName = sProcName;
        m_processTime.start();
#endif
    }
    inline void Restart(){
#ifdef QT_DEBUG
        m_processTime.restart();
#endif
    }
    inline void Output(){
#ifdef QT_DEBUG
        qDebug() << "---------------------------  <BENCHMARK>";
        qDebug() << qPrintable( QString("Process: [%1] Time: %2ms")
                    .arg(m_sProcessName)
                    .arg( (double) m_processTime.elapsed() , 0, 'f', 0) );
        qDebug() << "---------------------------  </BENCHMARK>";
#endif
    }
private:
    QTime m_processTime;
    QString m_sProcessName;

};

class LoopTimer {

public:
    inline LoopTimer(QString sLoopName){
#ifdef QT_DEBUG
        m_sLoopName = sLoopName;
        m_loopTimer.start();
        m_nLoopIterCount = 0;
        m_dLoopTimerSum = 0;
        m_dMaxTime = -1;
        m_dMinTime = -1;
#endif
    }
    inline void Restart(){
#ifdef QT_DEBUG
        m_loopTimer.restart();
#endif
    }

    inline void Tick(){
#ifdef QT_DEBUG

        double dElapsed = (double) m_loopTimer.elapsed();

        if ( m_dMaxTime == -1  || dElapsed > m_dMaxTime)
            m_dMaxTime = dElapsed;
        if ( m_dMinTime == -1  || m_dMinTime > dElapsed)
            m_dMinTime = dElapsed;

        m_dLoopTimerSum += dElapsed;
        m_nLoopIterCount++;
        m_loopTimer.restart();
#endif
    }

    inline void Output(){
#ifdef QT_DEBUG
        qDebug() << "---------------------------  <BENCHMARK>";
        qDebug() << qPrintable( QString("Loop: [%1] \n   Total Time: %2ms \n   Total Iter: %3\n   Avg Iter. Time: %4ms \n   Max Iter. Time: %5ms \n   Min Iter. time: %6ms\n")
                    .arg(m_sLoopName)
                    .arg(m_dLoopTimerSum, 0, 'f', 0)
                    .arg(m_nLoopIterCount)
                    .arg( (m_dLoopTimerSum / m_nLoopIterCount) , 0, 'f', 2)
                    .arg(m_dMaxTime, 0, 'f', 0)
                    .arg(m_dMinTime, 0, 'f', 0) );
        qDebug() << "---------------------------  </BENCHMARK>";
#endif
    }

private:
    QTime m_loopTimer;
    QString m_sLoopName;
    double m_dLoopTimerSum;
    int m_nLoopIterCount;
    double m_dMaxTime;
    double m_dMinTime;
};



}

#endif // BENCHMARK_H
