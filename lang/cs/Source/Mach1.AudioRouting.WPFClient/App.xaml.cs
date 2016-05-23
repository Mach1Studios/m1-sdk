using System;
using System.IO;
using System.Runtime.InteropServices;
using System.Windows;
using System.Windows.Threading;

namespace Mach1.AudioRouting.WPFClient
{
	/// <summary>
	/// Interaction logic for App.xaml
	/// </summary>
	public partial class App : Application
	{
		[DllImport("kernel32.dll", SetLastError = true)]
		[return: MarshalAs(UnmanagedType.Bool)]
		static extern bool AllocConsole();

		public static bool DebugModeEnabled { get; private set; }

		private static readonly object LockObject = new object();

		public App()
		{
			ReadArgs();
			Application.Current.DispatcherUnhandledException += Current_DispatcherUnhandledException;
		}

		private void ReadArgs()
		{
			string[] args = Environment.GetCommandLineArgs();
			if (args.Length > 1)
			{
				if (args[1] == "debug")
				{
					AllocConsole();
					DebugModeEnabled = true;
				}
			}
		}

		void Current_DispatcherUnhandledException(object sender,
			DispatcherUnhandledExceptionEventArgs e)
		{
			lock (LockObject)
			{
				MessageBox.Show(e.Exception.Message, "Error");
				string path = @"ErrorLog.txt";
				if (!File.Exists(path))
				{
					File.Create(path);
				}
				TextWriter tw = new StreamWriter(path, true);
				tw.WriteLine(DateTime.Now.ToString());
				tw.WriteLine("Exception: {0}", ExceptionToString(e.Exception));
				if (e.Exception.InnerException != null)
				{
					tw.WriteLine("Inner Exception: {0}", ExceptionToString(e.Exception.InnerException));
				}
				tw.WriteLine();
				tw.Close();
			}
		}

		private string ExceptionToString(Exception exception)
		{
			if (exception == null)
			{
				return "null";
			}
			return string.Format(
				"HResult:    {1}{0}" +
				"HelpLink:   {2}{0}" +
				"Message:    {3}{0}" +
				"Source:     {4}{0}"
				+ "StackTrace: {5}{0}" + "{0}",
				Environment.NewLine,
				exception.HResult,
				exception.HelpLink,
				exception.Message,
				exception.Source,
				exception.StackTrace);
		}
	}
}
