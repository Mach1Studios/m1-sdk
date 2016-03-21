using System;
using System.IO;
using System.Windows.Input;
using System.Windows.Threading;
using System.Xml.Serialization;
using Microsoft.Win32;
using Prism.Commands;
using Prism.Mvvm;

namespace Mach1.AudioRouting.WPFClient
{
	public class MainWindowViewModel : BindableBase
	{
		public ICommand LoadMultiFileCommand { get; private set; }
		public ICommand LoadOmniFileCommand { get; private set; }
		public ICommand PlayCommand { get; private set; }
		public ICommand StopCommand { get; private set; }
		public ICommand ClearOmniFileCommand { get; private set; }
		public ICommand LoadOrientationXMLCommand { get; private set; }
		public ICommand StartOrientationXMLCommand { get; private set; }

		public PeakInfoViewModel OmniPeakInfo { get; }
		public PeakInfoViewModel MasterPeakInfo { get; }

		public string MultiFileName
		{
			get { return _multiFileName; }
			set { SetProperty(ref _multiFileName, value); }
		}

		public string OmniFilename
		{
			get { return _omniFileName; }
			set { SetProperty(ref _omniFileName, value); }
		}

		public string OrientationXMLFilename
		{
			get { return _orientationXMLFilename; }
			set { SetProperty(ref _orientationXMLFilename, value); }
		}

		public double MasterVolume
		{
			get { return _masterVolume; }
			set
			{
				if (SetProperty(ref _masterVolume, value))
				{
					_audioProcessor.SetMultiVolume(value);
				}
			}
		}

		public double OmniVolume
		{
			get { return _omniVolume; }
			set
			{
				if (SetProperty(ref _omniVolume, value))
				{
					_audioProcessor.SetOmniVolume(value);
				}
			}
		}

		public double HorizontalAngle
		{
			get { return _horizontalAngle; }
			set
			{
				if (SetProperty(ref _horizontalAngle, value))
				{
					_audioProcessor.SetHorizontalAngle(value);
				}
			}
		}

		public double VerticalAngle
		{
			get { return _verticalAngle; }
			set
			{
				if (SetProperty(ref _verticalAngle, value))
				{
					_audioProcessor.SetVerticalAngle(value);
				}
			}
		}

		private string _multiFileName;
		private string _omniFileName;
		private string _orientationXMLFilename;
		private double _masterVolume = 1;
		private double _omniVolume = 1;
		private double _horizontalAngle;
		private double _verticalAngle;
		private readonly DispatcherTimer _timer = new DispatcherTimer();
		private OrientationList _orientationList;
		private int _currentOrientationIndex;

		private readonly IAudioProcessor _audioProcessor;

		public MainWindowViewModel()
		{
			_audioProcessor = new CSCoreAudioProcessor();
			OmniPeakInfo = new PeakInfoViewModel();
			MasterPeakInfo = new PeakInfoViewModel();
			InitializeCommands();
			_audioProcessor.MasterPeakCalculated += OnMasterPeakCalculated;
			_audioProcessor.OmniPeakCalculated += OnOmniPeakCalculated;
			_timer.Tick += (s, e) 
				=> UpdateOrientationFromXML();
		}

		private void InitializeCommands()
		{
			LoadMultiFileCommand = new DelegateCommand(LoadMultiFile);
			LoadOmniFileCommand = new DelegateCommand(LoadOmniFile, IsMultiFileLoaded);
			PlayCommand = new DelegateCommand(Play, IsMultiFileLoaded);
			StopCommand = new DelegateCommand(Stop, IsMultiFileLoaded);
			ClearOmniFileCommand = new DelegateCommand(ClearOmniFile,
				() => !string.IsNullOrEmpty(_omniFileName));
			LoadOrientationXMLCommand = new DelegateCommand(LoadOrientationXML);
			StartOrientationXMLCommand = new DelegateCommand(StartOrientationXML, 
				() => !string.IsNullOrEmpty(_orientationXMLFilename));
		}

		private void StartOrientationXML()
		{
			_currentOrientationIndex = 0;
			UpdateOrientationFromXML();
		}

		private void UpdateOrientationFromXML()
		{
			_timer.Stop();
			Orientation orientation = _orientationList.Orientations[_currentOrientationIndex];
			HorizontalAngle = orientation.Horizontal;
			VerticalAngle = orientation.Vertical;
			_currentOrientationIndex++;
			if (orientation.Duration > 0 && _currentOrientationIndex < _orientationList.Orientations.Count)
			{
				_timer.Interval = TimeSpan.FromSeconds(orientation.Duration);
				_timer.Start();
			}
		}

		private void LoadOrientationXML()
		{
			FileInfo file = GetFile("XML Files (*.xml)|*.xml");
			if (file != null)
			{
				_timer?.Stop();
				XmlSerializer serializer = new XmlSerializer(typeof(OrientationList));
				using (FileStream fileStream = new FileStream(file.FullName, FileMode.Open))
				{
					_orientationList = (OrientationList)serializer.Deserialize(fileStream);
				}
				OrientationXMLFilename = file.Name;
				((DelegateCommand)StartOrientationXMLCommand).RaiseCanExecuteChanged();
			}
		}

		private bool IsMultiFileLoaded()
		{
			return !string.IsNullOrEmpty(_multiFileName);
		}

		private void LoadMultiFile()
		{
			FileInfo file = GetFile(_audioProcessor.GetSupportedExtensionsFilterString());
			if (file != null)
			{
				_audioProcessor.InitializeMultiSource(file.FullName);
				MultiFileName = file.Name;
				MasterPeakInfo.Reset();
				((DelegateCommand)PlayCommand).RaiseCanExecuteChanged();
				((DelegateCommand)StopCommand).RaiseCanExecuteChanged();
				((DelegateCommand)LoadOmniFileCommand).RaiseCanExecuteChanged();
			}
		}

		private void LoadOmniFile()
		{
			FileInfo file = GetFile(_audioProcessor.GetSupportedExtensionsFilterString());
			if (file != null)
			{
				_audioProcessor.InitializeOmniSource(file.FullName);
				OmniFilename = file.Name;
				OmniPeakInfo.Reset();
				((DelegateCommand)ClearOmniFileCommand).RaiseCanExecuteChanged();
			}
		}

		private void Play()
		{
			_audioProcessor.Play();
		}

		private void Stop()
		{
			_audioProcessor.Stop();
		}

		private void ClearOmniFile()
		{
			_audioProcessor.InitializeOmniSource(string.Empty);
			OmniFilename = string.Empty;
			OmniPeakInfo.Reset();
			((DelegateCommand)ClearOmniFileCommand).RaiseCanExecuteChanged();
		}

		private void OnOmniPeakCalculated(object sender, PeakEventArgs peakEventArgs)
		{
			OmniPeakInfo.PeakValue = peakEventArgs.PeakValue;
			OmniPeakInfo.LeftPeakValue = peakEventArgs.ChannelPeakValues[0];
			OmniPeakInfo.RightPeakValue = peakEventArgs.ChannelPeakValues[1];
		}

		private void OnMasterPeakCalculated(object sender, PeakEventArgs peakEventArgs)
		{
			MasterPeakInfo.PeakValue = peakEventArgs.PeakValue;
			MasterPeakInfo.LeftPeakValue = peakEventArgs.ChannelPeakValues[0];
			MasterPeakInfo.RightPeakValue = peakEventArgs.ChannelPeakValues[1];
		}

		private FileInfo GetFile(string filter)
		{
			OpenFileDialog dialog = new OpenFileDialog();
			dialog.Filter = filter;
			if (dialog.ShowDialog() == true)
			{
				return new FileInfo(dialog.FileName);
			}
			return null;
		}
	}
}
