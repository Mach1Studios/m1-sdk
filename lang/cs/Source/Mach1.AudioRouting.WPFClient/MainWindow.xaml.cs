using System;
using System.ComponentModel;
using System.IO;
using System.Reflection;
using System.Windows;
using Microsoft.Win32;

namespace Mach1.AudioRouting.WPFClient
{
	public partial class MainWindow : Window
	{
		private readonly IAudioProcessor _audioProcessor;

		public MainWindow()
		{
			_audioProcessor = new CSCoreAudioProcessor();
			_audioProcessor.MasterPeakCalculated += OnMasterPeakCalculated;
			_audioProcessor.OmniPeakCalculated += OnOmniPeakCalculated;
			InitializeComponent();
			Title += GetVersionNumber();
		}

		private void OnOmniPeakCalculated(object sender, PeakEventArgs peakEventArgs)
		{
			Dispatcher.BeginInvoke(new Action(() =>
			{
				TextBlockOmniPeakValue.Text = peakEventArgs.PeakValue.ToString("F1");
				OmniLeftPeakBar.Value = peakEventArgs.ChannelPeakValues[0];
				OmniRightPeakBar.Value = peakEventArgs.ChannelPeakValues[1];
			}));
		}

		private void OnMasterPeakCalculated(object sender, PeakEventArgs peakEventArgs)
		{
			Dispatcher.BeginInvoke(new Action(() =>
			{
				TextBlockMultiPeakValue.Text = peakEventArgs.PeakValue.ToString("F1");
				MasterLeftPeakBar.Value = peakEventArgs.ChannelPeakValues[0];
				MasterRightPeakBar.Value = peakEventArgs.ChannelPeakValues[1];
			}));
		}

		private void ButtonLoadMultiFile_OnClick(object sender, RoutedEventArgs e)
		{
			FileInfo file = GetFile();
			if (file != null)
			{
				_audioProcessor.InitializeMultiSource(file.FullName);
				TextBlockMultiFileName.Text = file.Name;
				ButtonPlayMultiFile.IsEnabled = true;
				ButtonStopMultiFile.IsEnabled = true;
				ButtonLoadOmniFile.IsEnabled = true;
			}
		}

		private void ButtonLoadOmniFile_OnClick(object sender, RoutedEventArgs e)
		{
			FileInfo file = GetFile();
			if (file != null)
			{
				_audioProcessor.InitializeOmniSource(file.FullName);
				TextBlockOmniFileName.Text = file.Name;
				ButtonClearOmniFile.IsEnabled = true;
			}
		}

		private void ButtonPlayMultiFile_OnClick(object sender, RoutedEventArgs e)
		{
			_audioProcessor.Play();
		}

		private void ButtonStopMultiFile_OnClick(object sender, RoutedEventArgs e)
		{
			_audioProcessor.Stop();
		}

		private void ButtonClearOmniFile_OnClick(object sender, RoutedEventArgs e)
		{
			_audioProcessor.InitializeOmniSource(string.Empty);
			TextBlockOmniFileName.Text = string.Empty;
			TextBlockOmniPeakValue.Text = string.Empty;
			ButtonClearOmniFile.IsEnabled = false;
		}

		private void MainWindow_OnClosing(object sender, CancelEventArgs e)
		{
			_audioProcessor.Stop();
		}

		private FileInfo GetFile()
		{
			OpenFileDialog dialog = new OpenFileDialog();
			dialog.Filter = _audioProcessor.GetSupportedExtensionsFilterString();
			if (dialog.ShowDialog() == true)
			{
				return new FileInfo(dialog.FileName);
			}
			return null;
		}

		private void SliderMultiVolume_OnValueChanged(object sender, RoutedPropertyChangedEventArgs<double> e)
		{
			_audioProcessor.SetMultiVolume(e.NewValue);
		}

		private void SliderOmniVolume_OnValueChanged(object sender, RoutedPropertyChangedEventArgs<double> e)
		{
			_audioProcessor.SetOmniVolume(e.NewValue);
		}

		public static string GetVersionNumber()
		{
			Assembly assembly = Assembly.GetExecutingAssembly();
			Version version = AssemblyName.GetAssemblyName(assembly.Location).Version;
			return string.Format("{0}.{1}.{2}", version.Major, version.Minor, version.Build);
		}
	}
}
