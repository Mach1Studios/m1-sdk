using Prism.Mvvm;

namespace Mach1.AudioRouting.WPFClient
{
	public class PeakInfoViewModel : BindableBase
	{
		public float PeakValue
		{
			get { return _peakValue; }
			set { SetProperty(ref _peakValue, value); }
		}

		public float LeftPeakValue
		{
			get { return _leftPeakValue; }
			set { SetProperty(ref _leftPeakValue, value); }
		}

		public float RightPeakValue
		{
			get { return _rightPeakValue; }
			set { SetProperty(ref _rightPeakValue, value); }
		}

		private float _peakValue;
		private float _leftPeakValue;
		private float _rightPeakValue;

		public void Reset()
		{
			PeakValue = LeftPeakValue = RightPeakValue = 0;
		}
	}
}
