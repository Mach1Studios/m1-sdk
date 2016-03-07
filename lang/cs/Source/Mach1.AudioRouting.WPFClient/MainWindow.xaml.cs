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
		private string _multiFilePath;
		private string _omniFilePath;

		private readonly IAudioProcessor _audioProcessor;

		public MainWindow()
		{
			_audioProcessor = new CSCoreAudioProcessor();
			_audioProcessor.MultiPeakCalculated += OnMultiPeakCalculated;
			_audioProcessor.OmniPeakCalculated += OnOmniPeakCalculated;
			InitializeComponent();
			Title += GetVersionNumber();
		}

		private void OnOmniPeakCalculated(object sender, PeakEventArgs peakEventArgs)
		{
			Dispatcher.Invoke(delegate
			{
				TextBlockOmniPeakValue.Text = peakEventArgs.PeakValue.ToString("F1");
				OmniLeftPeakBar.Value = peakEventArgs.ChannelPeakValues[0];
				OmniRightPeakBar.Value = peakEventArgs.ChannelPeakValues[1];
			});
		}

		private void OnMultiPeakCalculated(object sender, PeakEventArgs peakEventArgs)
		{
			Dispatcher.Invoke(delegate
			{
				TextBlockMultiPeakValue.Text = peakEventArgs.PeakValue.ToString("F1");
				MasterLeftPeakBar.Value = peakEventArgs.ChannelPeakValues[0];
				MasterRightPeakBar.Value = peakEventArgs.ChannelPeakValues[1];
			});
		}

		private void ButtonLoadMultiFile_OnClick(object sender, RoutedEventArgs e)
		{
			FileInfo file = GetFile();
			if (file != null)
			{
				_multiFilePath = file.FullName;
				TextBlockMultiFileName.Text = file.Name;
				ButtonPlayMultiFile.IsEnabled = true;
				ButtonStopMultiFile.IsEnabled = true;
			}
		}

		private void ButtonLoadOmniFile_OnClick(object sender, RoutedEventArgs e)
		{
			FileInfo file = GetFile();
			if (file != null)
			{
				_omniFilePath = file.FullName;
				TextBlockOmniFileName.Text = file.Name;
				ButtonPlayOmniFile.IsEnabled = true;
				ButtonStopOmniFile.IsEnabled = true;
			}
		}

		private void ButtonPlayMultiFile_OnClick(object sender, RoutedEventArgs e)
		{
			_audioProcessor.PlayMulti(_multiFilePath);
		}

		private void ButtonPlayOmniFile_OnClick(object sender, RoutedEventArgs e)
		{
			_audioProcessor.PlayOmni(_omniFilePath);
		}

		private void ButtonStopMultiFile_OnClick(object sender, RoutedEventArgs e)
		{
			_audioProcessor.StopMulti();
		}

		private void ButtonStopOmniFile_OnClick(object sender, RoutedEventArgs e)
		{
			_audioProcessor.StopOmni();
		}

		private void MainWindow_OnClosing(object sender, CancelEventArgs e)
		{
			_audioProcessor.OmniPeakCalculated -= OnOmniPeakCalculated;
			_audioProcessor.MultiPeakCalculated -= OnMultiPeakCalculated;
			_audioProcessor.Dispose();
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
