using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Input;
using System.Windows.Threading;
using System.Xml.Serialization;
using Microsoft.Win32;
using Prism.Commands;
using Prism.Mvvm;

namespace Mach1.AudioRouting.WPFClient
{
	public enum MultichannelInputType
	{
		SingleFile,
		FourPairs,
		EightPairs
	}

	public class MainWindowViewModel : BindableBase
	{
		public ICommand LoadMultiFileCommand { get; private set; }
		public ICommand LoadOmniFileCommand { get; private set; }
		public ICommand PlayCommand { get; private set; }
		public ICommand StopCommand { get; private set; }
		public ICommand ClearOmniFileCommand { get; private set; }
		public ICommand ClearMultiFilesCommand { get; private set; }
		public ICommand LoadOrientationXMLCommand { get; private set; }
		public ICommand StartOrientationXMLCommand { get; private set; }

		public PeakInfoViewModel OmniPeakInfo { get; }
		public PeakInfoViewModel MasterPeakInfo { get; }

		public MultichannelInputType SelectedMultichannelInputType
		{
			get { return _selectedMultichannelInputType; }
			set
			{
				SetProperty(ref _selectedMultichannelInputType, value);
				UpdateInputFileList(value);
			}
		}

		public List<InputFileViewModel> ChannelPairs { get; set; }

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
					Dispatcher.CurrentDispatcher.InvokeAsync(
						async () => _audioProcessor.SetHorizontalAngle(value));
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
					Dispatcher.CurrentDispatcher.InvokeAsync(
						async () => _audioProcessor.SetVerticalAngle(value));
				}
			}
		}

		public double TiltAngle
		{
			get { return _tiltAngle; }
			set
			{
				if (SetProperty(ref _tiltAngle, value))
				{
					Dispatcher.CurrentDispatcher.InvokeAsync(
						async () => _audioProcessor.SetTiltAngle(value));
				}
			}
		}

		public bool IsOmniFileLoading
		{
			get { return _isOmniFileLoading; }
			set { SetProperty(ref _isOmniFileLoading, value); }
		}

		private string _omniFileName;
		private string _orientationXMLFilename;
		private double _masterVolume = 1;
		private double _omniVolume = 1;
		private double _horizontalAngle;
		private double _verticalAngle;
		private double _tiltAngle;
		private readonly DispatcherTimer _timer = new DispatcherTimer();
		private OrientationList _orientationList;
		private int _currentOrientationIndex;
		private MultichannelInputType _selectedMultichannelInputType;
		private bool _isOmniFileLoading;

		private readonly IAudioProcessor _audioProcessor;

		public MainWindowViewModel()
		{
			SelectedMultichannelInputType = MultichannelInputType.SingleFile;
			_audioProcessor = new CSCoreAudioProcessor(App.DebugModeEnabled);
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
			LoadMultiFileCommand = new DelegateCommand<object>(LoadMultiInput);
			LoadOmniFileCommand = new DelegateCommand(LoadOmniInput, IsMultiInputLoaded);
			PlayCommand = new DelegateCommand(Play, IsMultiInputLoaded);
			StopCommand = new DelegateCommand(Stop, IsMultiInputLoaded);
			ClearOmniFileCommand = new DelegateCommand(ClearOmniFile,
				() => !string.IsNullOrEmpty(_omniFileName));
			ClearMultiFilesCommand = new DelegateCommand(ClearMultiFiles);
			LoadOrientationXMLCommand = new DelegateCommand(LoadOrientationXML);
			StartOrientationXMLCommand = new DelegateCommand(StartOrientationXML,
				() => !string.IsNullOrEmpty(_orientationXMLFilename));
		}

		private void ClearMultiFiles()
		{
			UpdateInputFileList(_selectedMultichannelInputType);
		}

		private void UpdateInputFileList(MultichannelInputType inputType)
		{
			switch (inputType)
			{
				case MultichannelInputType.SingleFile:
					ChannelPairs = new List<InputFileViewModel>
					{
						new InputFileViewModel {Header = "Multichannel", Id = 1}
					};
					break;
				case MultichannelInputType.FourPairs:
					ChannelPairs = new List<InputFileViewModel>
					{
						new InputFileViewModel {Header = "Pair 1", Id = 1},
						new InputFileViewModel {Header = "Pair 2", Id = 2},
						new InputFileViewModel {Header = "Pair 3", Id = 3},
						new InputFileViewModel {Header = "Pair 4", Id = 4}
					};
					break;
				case MultichannelInputType.EightPairs:
					ChannelPairs = new List<InputFileViewModel>
					{
						new InputFileViewModel {Header = "Pair 1", Id = 1},
						new InputFileViewModel {Header = "Pair 2", Id = 2},
						new InputFileViewModel {Header = "Pair 3", Id = 3},
						new InputFileViewModel {Header = "Pair 4", Id = 4},
						new InputFileViewModel {Header = "Pair 5", Id = 5},
						new InputFileViewModel {Header = "Pair 6", Id = 6},
						new InputFileViewModel {Header = "Pair 7", Id = 7},
						new InputFileViewModel {Header = "Pair 8", Id = 8},
					};
					break;
				default:
					throw new ArgumentOutOfRangeException(nameof(inputType), inputType, null);
			}
			OnPropertyChanged(() => ChannelPairs);
			Stop();
			((DelegateCommand)PlayCommand)?.RaiseCanExecuteChanged();
			((DelegateCommand)StopCommand)?.RaiseCanExecuteChanged();
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

		private bool IsMultiInputLoaded()
		{
			return ChannelPairs.All(p => !string.IsNullOrEmpty(p.FileName));
		}

		private async void LoadMultiInput(object id)
		{
			FileInfo file = GetFile(_audioProcessor.GetSupportedExtensionsFilterString());
			if (file != null)
			{
				InputFileViewModel inputFileViewModel = ChannelPairs.First(p => p.Id == (int)id);
				inputFileViewModel.FileName = file.Name;
				inputFileViewModel.FilePath = file.FullName;
				if (ChannelPairs.All(p => !string.IsNullOrEmpty(p.FileName)))
				{
					foreach (InputFileViewModel channelPair in ChannelPairs)
					{
						channelPair.IsLoading = true;
					}
					await Task.Run(() =>
					{
						_audioProcessor.LoadMultiSource(ChannelPairs.Select(p => p.FilePath).ToList());
					});
					foreach (InputFileViewModel channelPair in ChannelPairs)
					{
						channelPair.IsLoading = false;
					}
					MasterPeakInfo.Reset();
					((DelegateCommand)PlayCommand).RaiseCanExecuteChanged();
					((DelegateCommand)StopCommand).RaiseCanExecuteChanged();
					((DelegateCommand)LoadOmniFileCommand).RaiseCanExecuteChanged();
				}
			}
		}

		private async void LoadOmniInput()
		{
			FileInfo file = GetFile(_audioProcessor.GetSupportedExtensionsFilterString());
			if (file != null)
			{
				try
				{
					IsOmniFileLoading = true;
					await Task.Run(() => { _audioProcessor.LoadOmniSource(file.FullName); });
				}
				catch (Exception ex)
				{
					MessageBox.Show(ex.Message, "Error");
					IsOmniFileLoading = false;
					return;
				}
				IsOmniFileLoading = false;
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
			_audioProcessor?.Stop();
		}

		private void ClearOmniFile()
		{
			_audioProcessor.LoadOmniSource(string.Empty);
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
