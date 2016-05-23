using Prism.Mvvm;

namespace Mach1.AudioRouting.WPFClient
{
	public class InputFileViewModel : BindableBase
	{
		public int Id { get; set; }

		public string Header { get; set; }

		public string FileName
		{
			get { return _fileName; }
			set { SetProperty(ref _fileName, value); }
		}

		public string FilePath { get; set; }

		public bool IsLoading
		{
			get { return _isLoading; }
			set { SetProperty(ref _isLoading, value); }
		}

		private string _fileName;
		private bool _isLoading;
	}
}
