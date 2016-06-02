using System;
using System.Reflection;
using System.Windows;
using HelixToolkit.Wpf;

namespace Mach1.AudioRouting.WPFClient
{
	public partial class MainWindow : Window
	{
		public MainWindow()
		{
			DataContext = new MainWindowViewModel();
			InitializeComponent();
			Title += GetVersionNumber();
		}

		public static string GetVersionNumber()
		{
			Assembly assembly = Assembly.GetExecutingAssembly();
			Version version = AssemblyName.GetAssemblyName(assembly.Location).Version;
			return string.Format("{0}.{1}.{2}", version.Major, version.Minor, version.Build);
		}
	}
}
