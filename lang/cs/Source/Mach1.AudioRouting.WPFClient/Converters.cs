using System;
using System.Globalization;
using System.Windows;
using System.Windows.Data;

namespace Mach1.AudioRouting.WPFClient
{
	public class TrueToVisibleConverter : IValueConverter
	{
		public object Convert(object value, Type targetType, object parameter, CultureInfo culture)
		{
			bool input = (bool)value;
			Visibility output = input ? Visibility.Visible : Visibility.Collapsed;
			return output;
		}

		public object ConvertBack(object value, Type targetType, object parameter, CultureInfo culture)
		{
			throw new NotImplementedException();
		}
	}

	public class FalseToVisibleConverter : IValueConverter
	{
		public object Convert(object value, Type targetType, object parameter, CultureInfo culture)
		{
			bool input = (bool)value;
			Visibility output = !input ? Visibility.Visible : Visibility.Collapsed;
			return output;
		}

		public object ConvertBack(object value, Type targetType, object parameter, CultureInfo culture)
		{
			throw new NotImplementedException();
		}
	}
}
