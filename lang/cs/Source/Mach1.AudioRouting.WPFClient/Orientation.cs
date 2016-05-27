using System.Collections.Generic;
using System.Xml.Serialization;

namespace Mach1.AudioRouting.WPFClient
{
	[XmlRoot("OrientationList")]
	public class OrientationList
	{
		[XmlElement("Orientation")]
		public List<Orientation> Orientations { get; set; }
	}

	public class Orientation
	{
		public Orientation()
		{
			Duration = -1;
		}

		[XmlAttribute("Horizontal")]
		public double Horizontal { get; set; }
		[XmlAttribute("Vertical")]
		public double Vertical { get; set; }
		[XmlAttribute("Duration")]
		public double Duration { get; set; }
	}
}
