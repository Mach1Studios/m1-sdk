using System.ComponentModel;
using System.IO;
using System.Windows;
using Microsoft.Win32;

namespace Mach1.AudioRouting.WPFClient
{
	public partial class MainWindow : Window
	{
		private string _multichannelFilePath;
		private string _omniFilePath;

		private readonly IAudioProcessor _audioProcessor;

		public MainWindow()
		{
			InitializeComponent();
			_audioProcessor = new CSCoreAudioProcessor();
		}

		private void ButtonLoadMultichannelFile_OnClick(object sender, RoutedEventArgs e)
		{
			FileInfo file = GetFile();
			if (file != null)
			{
				_multichannelFilePath = file.FullName;
				TextBlockMultichannelFileName.Text = file.Name;
				ButtonPlayMultichannelFile.IsEnabled = true;
				ButtonStopMultichannelFile.IsEnabled = true;
			}
		}

		private void ButtonLoadOmniFile_OnClick(object sender, RoutedEventArgs e)
		{
			FileInfo file = GetFile();
			if (file != null)
			{
				_omniFilePath = file.FullName;
				TextBlockOmniFileName.Text = file.Name;
			}
		}

		private void ButtonPlayMultichannelFile_OnClick(object sender, RoutedEventArgs e)
		{
			_audioProcessor.Play(_multichannelFilePath);
		}

		private void ButtonStopMultichannelFile_OnClick(object sender, RoutedEventArgs e)
		{
			_audioProcessor.Stop();
		}

		private void MainWindow_OnClosing(object sender, CancelEventArgs e)
		{
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
	}
}
