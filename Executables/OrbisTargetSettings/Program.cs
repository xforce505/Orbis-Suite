﻿using System;
using System.Collections.Generic;
using System.Linq;
using System.Threading.Tasks;
using System.Windows.Forms;
using System.Threading;

namespace OrbisTargetSettings
{
    static class Program
    {
        /// <summary>
        /// The main entry point for the application.
        /// </summary>
        [STAThread]
        static void Main()
        {
            bool MutexCreated = true;
            Mutex mutex = new Mutex(true, "{45FA8D0A-A56B-4D25-8F5D-6D28BAD65309}", out MutexCreated);

            if (MutexCreated && mutex.WaitOne(TimeSpan.Zero, true))
            {
                Application.EnableVisualStyles();
                Application.SetCompatibleTextRenderingDefault(false);
                Application.Run(new OrbisTargetSettings());
                mutex.ReleaseMutex();
            }
            else
            {
                // send our Win32 message to make the currently running instance
                // jump on top of all the other windows
                NativeMethods.PostMessage(
                    (IntPtr)NativeMethods.HWND_BROADCAST,
                    NativeMethods.WM_TARGETSETTINGS,
                    IntPtr.Zero,
                    IntPtr.Zero);
            }
        }
    }
}
