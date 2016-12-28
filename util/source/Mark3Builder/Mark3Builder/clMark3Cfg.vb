Public Class clMark3Cfg
    Private m_bDynamicThreads As Boolean
    Private m_bNamedThreads As Boolean
    Private m_bSafeUnlinking As Boolean
    Private m_uiTransactionQueueSize As UInteger

    Private m_bUseTimers As Boolean
    Private m_bTicklessTimers As Boolean
    Private m_bUseSleep As Boolean
    Private m_bUseQuanta As Boolean
    Private m_uiDefaultQuanta As UInteger

    Private m_bUseSemaphores As Boolean
    Private m_bUseMutexes As Boolean
    Private m_bUseEventFlag As Boolean
    Private m_bUseMessages As Boolean
    Private m_uiMessageQueueSize As UInteger

    Private m_bUseAtomic As Boolean
    Private m_bUseDebug As Boolean
    Private m_bUseProfilingTimers As Boolean
    Private m_bUseDrivers As Boolean

    Public Sub SetUseDynamic(ByVal bDynamic_ As Boolean)
        m_bDynamicThreads = bDynamic_
    End Sub
    Public Sub SetUseNamedThreads(ByVal bNamedThreads_ As Boolean)
        m_bNamedThreads = bNamedThreads_
    End Sub
    Public Sub SetUseSafeUnlinking(ByVal bSafeUnlinking_ As Boolean)
        m_bSafeUnlinking = bSafeUnlinking_
    End Sub
    Public Sub SetTransactionQueueSize(ByVal uiSize_ As UInteger)
        m_uiTransactionQueueSize = uiSize_
    End Sub

    Public Sub SetUseTimers(ByVal bUseTimers_ As Boolean)
        m_bUseTimers = bUseTimers_
    End Sub
    Public Sub SetUseTickless(ByVal bUseTickless_ As Boolean)
        m_bTicklessTimers = bUseTickless_
    End Sub
    Public Sub SetUseSleep(ByVal bUseSleep_ As Boolean)
        m_bUseSleep = bUseSleep_
    End Sub
    Public Sub SetUseQuanta(ByVal bUseQuanta_ As Boolean)
        m_bUseQuanta = bUseQuanta_
    End Sub
    Public Sub SetDefaultQuanta(ByVal uiQuanta_ As UInteger)
        m_uiDefaultQuanta = uiQuanta_
    End Sub

    Public Sub SetUseSemaphores(ByVal bUseSemaphores_ As Boolean)
        m_bUseSemaphores = bUseSemaphores_
    End Sub
    Public Sub SetUseMutex(ByVal bUseMutex_ As Boolean)
        m_bUseMutexes = bUseMutex_
    End Sub
    Public Sub SetUseEventFlag(ByVal bUseEvent_ As Boolean)
        m_bUseEventFlag = bUseEvent_
    End Sub
    Public Sub SetUseMessages(ByVal bUseMessages_ As Boolean)
        m_bUseMessages = bUseMessages_
    End Sub
    Public Sub SetGlobalMessageQueue(ByVal uiMessageQueueSize_ As UInteger)
        m_uiMessageQueueSize = uiMessageQueueSize_
    End Sub

    Public Sub SetUseAtomic(ByVal bUseAtomic_ As Boolean)
        m_bUseAtomic = bUseAtomic_
    End Sub
    Public Sub SetUseDebug(ByVal bUseDebug_ As Boolean)
        m_bUseDebug = bUseDebug_
    End Sub
    Public Sub SetUseDrivers(ByVal bUseDriver_ As Boolean)
        m_bUseDrivers = bUseDriver_
    End Sub
    Public Sub SetUseProfilingTimers(ByVal bUseProfiling_ As Boolean)
        m_bUseProfilingTimers = bUseProfiling_
    End Sub


    Public Function GetUseDynamic() As Boolean
        Return m_bDynamicThreads
    End Function
    Public Function GetUseNamedThreads() As Boolean
        Return m_bNamedThreads
    End Function
    Public Function GetUseSafeUnlinking() As Boolean
        Return m_bSafeUnlinking
    End Function
    Public Function GetTransactionQueueSize() As UInteger
        Return m_uiTransactionQueueSize
    End Function

    Public Function GetUseTimers() As Boolean
        Return m_bUseTimers
    End Function
    Public Function GetUseTickless() As Boolean
        Return m_bTicklessTimers
    End Function
    Public Function GetUseSleep() As Boolean
        Return m_bUseSleep
    End Function
    Public Function GetUseQuanta() As Boolean
        Return m_bUseQuanta
    End Function
    Public Function GetDefaultQuanta() As UInteger
        Return m_uiDefaultQuanta
    End Function

    Public Function GetUseSemaphores() As Boolean
        Return m_bUseSemaphores
    End Function
    Public Function GetUseMutex() As Boolean
        Return m_bUseMutexes
    End Function
    Public Function GetUseEventFlag() As Boolean
        Return m_bUseEventFlag
    End Function
    Public Function GetUseMessages() As Boolean
        Return m_bUseMessages
    End Function
    Public Function GetGlobalMessageQueue() As UInteger
        Return m_uiMessageQueueSize
    End Function

    Public Function GetUseAtomic() As Boolean
        Return m_bUseAtomic
    End Function
    Public Function GetUseDebug() As Boolean
        Return m_bUseDebug
    End Function
    Public Function GetUseDrivers() As Boolean
        Return m_bUseDrivers
    End Function
    Public Function GetUseProfilingTimers() As UInteger
        Return m_bUseProfilingTimers
    End Function

    Public Function Validate() As Boolean
        If (m_bUseMessages = True And m_bUseSemaphores = False) Then

            If MessageBox.Show("Message Queue support is dependent on Semaphores, set and continue?", "Feature Dependency", MessageBoxButtons.OKCancel) = DialogResult.Cancel Then
                m_bUseMessages = False
                Return False
            End If

            m_bUseSemaphores = True
        End If
        If (m_bUseQuanta = True And m_bUseTimers = False) Then
            If MessageBox.Show("Round-robin scheduler requires Timers to be enabled, set and continue?", "Feature Dependency", MessageBoxButtons.OKCancel) = DialogResult.Cancel Then
                m_bUseQuanta = False
                Return False
            End If

            m_bUseTimers = True
        End If
        If (m_bTicklessTimers = True And m_bUseTimers = False) Then
            If MessageBox.Show("Tickless Timers require timers to be enabled, set and continue?", "Feature Dependency", MessageBoxButtons.OKCancel) = DialogResult.Cancel Then
                m_bTicklessTimers = False
                Return False
            End If

            m_bUseTimers = True
        End If
        If ((m_bUseSleep = True And m_bUseTimers = False) Or _
            (m_bUseSleep = True And m_bUseSemaphores = False)) Then
            If MessageBox.Show("Thread Sleep requires Timers and Semaphores to be enabled, set and continue?", "Feature Dependency", MessageBoxButtons.OKCancel) = DialogResult.Cancel Then
                m_bUseSleep = False
                Return False
            End If

            m_bUseTimers = True
            m_bUseSemaphores = True
        End If

        If (m_bUseQuanta = True And m_bUseTimers = False) Then
            If MessageBox.Show("Round-robin scheduler requires Timers to be enabled, set and continue?", "Feature Dependency", MessageBoxButtons.OKCancel) = DialogResult.Cancel Then
                m_bUseQuanta = False
                Return False
            End If

            m_bUseTimers = True
        End If

        If (m_bUseQuanta = True And m_uiDefaultQuanta < 2) Then
            If MessageBox.Show("Round-robin scheduler requires minimum Quanta = 2ms, set and continue?", "Feature Dependency", MessageBoxButtons.OKCancel) = DialogResult.Cancel Then
                m_bUseQuanta = False
                Return False
            End If

            m_uiDefaultQuanta = 2
        End If

        If (m_uiTransactionQueueSize < 2) Then
            If MessageBox.Show("Minimum Transaction Queue Size = 2, set and continue?", "Feature Dependency", MessageBoxButtons.OKCancel) = DialogResult.Cancel Then
                Return False
            End If

            m_uiTransactionQueueSize = 2
        End If

        Return True
    End Function

    Public Sub SetDefaults()
        m_bDynamicThreads = True
        m_bNamedThreads = False
        m_bSafeUnlinking = False
        m_uiTransactionQueueSize = 3

        m_bUseTimers = True
        m_bTicklessTimers = True
        m_bUseSleep = True
        m_bUseQuanta = False
        m_uiDefaultQuanta = 4

        m_bUseSemaphores = True
        m_bUseMutexes = True
        m_bUseEventFlag = False
        m_bUseMessages = True
        m_uiMessageQueueSize = 8

        m_bUseAtomic = False
        m_bUseDrivers = True
        m_bUseDebug = False
        m_bUseProfilingTimers = False
    End Sub
    Public Function GetEstimatePercent() As Double
        Return EstimateSize() / 12454.0
    End Function
    Public Function EstimateSize() As UInteger
        Dim ret As UInteger
        ret = 0

        '-  Synchronization Objects - Base Class............ : 266 Bytes
        '-  Fundamental Kernel Linked-List Classes.......... : 610 Bytes
        '-  Fundamental Kernel Thread-list Data Structures.. : 212 Bytes
        '-  Mark3 Kernel Base Class......................... : 80 Bytes
        '-  Circular Logging Buffer Base Class.............. : 524 Bytes
        '-  Kernel Transaction Queues....................... : 308 Bytes
        '' Base size of the kernel
        ret = 266 + 610 + 212 + 80 + 524 + 308

        '-  AVR Port - Basic Threading Support.............. : 528 Bytes
        '-  AVR Port - Kernel Interrupt Implemenation....... : 56 Bytes
        '-  AVR Port - Kernel Timer Implementation.......... : 338 Bytes
        '-  AVR Port - Profiling Timer Implementation....... : 256 Bytes
        '' Size of AVR Port code
        ret += 528 + 56 + 338 + 256

        '-'  Thread Implementation........................... : 1441 Bytes
        ret += 1441

        '-'  Thread Scheduling............................... : 499 Bytes
        ret += 499

        If m_bDynamicThreads = False Then
            ret -= 50
        End If

        If m_bNamedThreads = False Then
            ret -= 32
        End If

        If m_bUseSleep = False Then
            ret -= 64
        End If

        If m_bSafeUnlinking = False Then
            ret -= 110
        End If

        '-  Software Timer Implementation................... : 1035 Bytes
        If m_bUseTimers = True Then
            If m_bTicklessTimers = True Then
                ret += 1035
            Else
                ret += 610
            End If
        End If

        '-  Round-Robin Scheduling Support.................. : 299 Bytes
        If m_bUseQuanta = True Then
            ret += 299
        End If

        '-'  Semaphore (Synchronization Object).............. : 868 Bytes
        If m_bUseSemaphores = True Then
            ret += 868
        End If

        '-  Mutex (Synchronization Object).................. : 1004 Bytes
        If m_bUseMutexes = True Then
            ret += 1004
        End If

        '-  Synchronization Object - Event Flag............. : 1160 Bytes
        If m_bUseEventFlag = True Then
            ret += 1160
        End If
        '-  Message-based IPC............................... : 500 Bytes
        If m_bUseMessages = True Then
            ret += 500
        End If

        '-  Atomic Operations............................... : 478 Bytes
        If m_bUseAtomic = True Then
            ret += 478
        End If

        '-  Device Driver Framework (including /dev/null)... : 236 Bytes
        If m_bUseDrivers = True Then
            ret += 236
        End If
        '-  Runtime Kernel Trace Implementation............. : 0 Bytes
        If m_bUseDebug = True Then
            ret += 1200
        End If
        '-  Performance-profiling timers.................... : 556 Bytes
        If m_bUseProfilingTimers = True Then
            ret += 556
        End If

        Return ret
    End Function
    Public Function GetFeatureSummary() As String
        Dim ret As String
        ret = "Threading/Scheduling:" + vbNewLine
        ret += "  Pre-emptive multithreading (required)" + vbNewLine
        ret += "  Fixed time thread Scheduler (required)" + vbNewLine
        ret += "  CPU-specific kernel port (required)" + vbNewLine
        ret += "  Basic kernel functions (required)" + vbNewLine

        If m_bDynamicThreads = True Then
            ret += "  Dynamic Threading" + vbNewLine
        End If

        If m_bNamedThreads = True Then
            ret += "  Named Threads" + vbNewLine
        End If

        If m_bSafeUnlinking = True Then
            ret += "  Safe Unlinking" + vbNewLine
        End If

        ret += "  Transaction Queue Size: " + Str(m_uiTransactionQueueSize) + vbNewLine

        ret += vbNewLine + "Timers:" + vbNewLine

        If m_bUseTimers = True Then
            ret += "  Software Timer Support"
            If m_bTicklessTimers = True Then
                ret += " - Tickless"
            Else
                ret += " - Fixed-frequency Tick"
            End If
            ret += vbNewLine
        End If

        If m_bUseSleep = True Then
            ret += "  Thread Sleep" + vbNewLine
        End If

        If m_bUseQuanta = True Then
            ret += "  Round-robin Scheduling with default quanta of " + Str(m_uiDefaultQuanta) + "(ms)" + vbNewLine
        End If

        ret += vbNewLine + "Synchronization:" + vbNewLine

        If m_bUseSemaphores = True Then
            ret += "  Semaphores (Binary and Counting)" + vbNewLine
        End If

        If m_bUseMutexes = True Then
            ret += "  Mutexes, with priority inheritence" + vbNewLine
        End If

        If m_bUseEventFlag = True Then
            ret += "  Event flag (16-bits/group)" + vbNewLine
        End If

        If m_bUseMessages = True Then
            ret += "  Message-based IPC, with global queue of " + Str(m_uiMessageQueueSize) + " messages" + vbNewLine
        End If

        ret += vbNewLine + "Other Features:" + vbNewLine

        If m_bUseAtomic = True Then
            ret += "  Atomic Operations (Add/Subtract/Set/Test-and-set)" + vbNewLine
        End If
        If m_bUseDrivers = True Then
            ret += "  Unified Device Drivers API"
        End If
        If m_bUseDebug = True Then
            ret += "  Instrumentation/Debugging Support"
        End If
        If m_bUseProfilingTimers = True Then
            ret += "  High-resolution code profiling timers"
        End If
        ret += vbNewLine
        Return ret
    End Function

    Public Function GenerateHeader() As String
        Dim ret As String

        ret = "//Mark3 Realtime Kernel - (c) 2013 All Rights Reserved" + vbNewLine
        ret += "//This file is AUTOGENERATED - DO NOT MODIFY!!" + vbNewLine
        ret += "#ifndef __MARK3_CFG_H__" + vbNewLine
        ret += "#define __MARK3_CFG_H__" + vbNewLine

        ret += "#define KERNEL_USE_DYNAMIC_THREADS "
        If m_bDynamicThreads = True Then
            ret += "(1)"
        Else
            ret += "(0)"
        End If
        ret += vbNewLine

        ret += "#define KERNEL_USE_THREADNAME "
        If m_bNamedThreads = True Then
            ret += "(1)"
        Else
            ret += "(0)"
        End If
        ret += vbNewLine

        ret += "#define SAFE_UNLINK "
        If m_bSafeUnlinking = True Then
            ret += "(1)"
        Else
            ret += "(0)"
        End If
        ret += vbNewLine

        ret += "#define TRANSACTION_QUEUE_SIZE (" + Str(m_uiTransactionQueueSize) + ")" + vbNewLine

        ret += "#define KERNEL_USE_TIMERS "
        If m_bUseTimers = True Then
            ret += "(1)" + vbNewLine
            ret += "#define KERNEL_TIMERS_TICKLESS "
            If m_bTicklessTimers = True Then
                ret += "(1)" + vbNewLine
            Else
                ret += "(0)" + vbNewLine
            End If            
        Else
            ret += "(0)" + vbNewLine
            ret += "#define KERNEL_TIMERS_TICKLESS  (0)" + vbNewLine            
        End If

        ret += "#define KERNEL_USE_SLEEP "
        If m_bUseSleep = True Then
            ret += "(1)" + vbNewLine
        Else
            ret += "(0)" + vbNewLine
        End If

        ret += "#define KERNEL_USE_QUANTUM"
        If m_bUseQuanta = True Then
            ret += "(1)" + vbNewLine
            ret += "#define THREAD_QUANTUM_DEFAULT (" + Str(m_uiDefaultQuanta) + ")" + vbNewLine
        Else
            ret += "(0)" + vbNewLine
            ret += "#define THREAD_QUANTUM_DEFAULT (0)" + vbNewLine
        End If

        ret += "#define KERNEL_USE_SEMAPHORE "
        If m_bUseSemaphores = True Then
            ret += "(1)" + vbNewLine
        Else
            ret += "(0)" + vbNewLine
        End If


        ret += "#define KERNEL_USE_MUTEX "
        If m_bUseMutexes = True Then
            ret += "(1)" + vbNewLine
        Else
            ret += "(0)" + vbNewLine
        End If

        ret += "#define KERNEL_USE_EVENTFLAG "
        If m_bUseEventFlag = True Then
            ret += "(1)" + vbNewLine
        Else
            ret += "(0)" + vbNewLine
        End If


        ret += "#define KERNEL_USE_MESSAGE "
        If m_bUseMessages = True Then
            ret += "(1)" + vbNewLine
            ret += "#define GLOBAL_MESSAGE_POOL_SIZE (" + Str(m_uiMessageQueueSize) + ")" + vbNewLine
        Else
            ret += "(0)" + vbNewLine
            ret += "#define GLOBAL_MESSAGE_POOL_SIZE (0)" + vbNewLine
        End If

        ret += "#define KERNEL_USE_ATOMIC "
        If m_bUseAtomic = True Then
            ret += "(1)" + vbNewLine
        Else
            ret += "(0)" + vbNewLine
        End If

        ret += "#define KERNEL_USE_DRIVER "
        If m_bUseDrivers = True Then
            ret += "(1)" + vbNewLine
        Else
            ret += "(0)" + vbNewLine
        End If

        ret += "#define KERNEL_USE_DEBUG "
        If m_bUseDebug = True Then
            ret += "(1)" + vbNewLine
        Else
            ret += "(0)" + vbNewLine
        End If

        ret += "#define KERNEL_USE_PROFILER "
        If m_bUseProfilingTimers = True Then
            ret += "(1)" + vbNewLine
        Else
            ret += "(0)" + vbNewLine
        End If

        ret += vbNewLine
        ret += "#endif //__MARK3_CFG_H__" + vbNewLine
        Return ret
    End Function
End Class
