using UnityEngine;
using System;
using System.Text.RegularExpressions;
using System.Linq;
using Ovr;

/// <summary>
/// Determines whether various features work based on the current Unity version.
/// </summary>
public static class OVRUnityVersionChecker
{
	private static OVRUnityVersion cachedVersion;
	private static bool cachedVersionInitialized;
	/// <summary>
	/// The current version of Unity.
	/// </summary>
	public static OVRUnityVersion version
	{
		get
		{
			if (!cachedVersionInitialized)
			{
				cachedVersion = new OVRUnityVersion(Application.unityVersion);
				cachedVersionInitialized = true;
			}

			return cachedVersion;
		}
	}

	/// <summary>
	/// If true, Unity can use OculusInitPlugin instead of _DirectToRift.exe.
	/// </summary>
	public static bool hasPreInitSupport
	{
		get
		{
			return (version.major == 4 && version >= "4.6.3p2");
		}
	}

	/// <summary>
	/// If true, Unity supports D3D9 exclusive fullscreen mode for proper DK2 vsync.
	/// </summary>
	public static bool hasD3D9ExclusiveModeSupport
	{
		get
		{
			bool isUnsupportedUnityVersion =
				   (version >= "4.5.2p2" && version < "4.5.5b1")
				|| (version >= "4.6.0b1" && version < "4.6.0b21");

			return !isUnsupportedUnityVersion;
		}
	}
	
	/// <summary>
	/// If true, Unity supports D3D11 exclusive fullscreen mode for proper DK2 vsync.
	/// </summary>
	public static bool hasD3D11ExclusiveModeSupport
	{
		get
		{
			bool isUnsupportedUnityVersion =
			       (version >= "4.5.2p2" && version < "4.5.5p3")
				|| (version >= "4.6.0b1" && version < "4.6.0b22")
				|| (version >= "5.0.0b1" && version < "5.0.0b19");

			return !isUnsupportedUnityVersion;
		}
	}
}

/// <summary>
/// Represents a Unity version of the form <major>.<minor>.<change><type><release>, such as 5.0.0p1.
/// </summary>
public struct OVRUnityVersion : IComparable<OVRUnityVersion>
{
	public int major { get; private set; }
	public int minor { get; private set; }
	public int change { get; private set; }
	public char type { get; private set; }
	public int release { get; private set; }

	public OVRUnityVersion(string unityVersion)
    : this()
	{
		// Split the version string at non-numbers.
		string nonNumbers = "[^0-9]";
		string[] tokens = Regex.Split(unityVersion, nonNumbers);

		// Sanity check if nothing went wrong
		bool validUnityVersion = true;

		int parsedMajor = 0;
		int parsedMinor = 0;
		int parsedChange = 0;
		char parsedType = 'a';
		int parsedRelease = 1;

		if (tokens.Length == 4)
		{
			validUnityVersion &= int.TryParse(tokens[0], out parsedMajor);
			validUnityVersion &= int.TryParse(tokens[1], out parsedMinor);
			validUnityVersion &= int.TryParse(tokens[2], out parsedChange);
			validUnityVersion &= int.TryParse(tokens[3], out parsedRelease);

			char lastChar = unityVersion.LastOrDefault(char.IsLetter);
			if (lastChar != '\0')
				parsedType = lastChar;
			else
				validUnityVersion = false;
		}
		else
		{
			validUnityVersion = false;
		}

		major = parsedMajor;
		minor = parsedMinor;
		change = parsedChange;
		type = parsedType;
		release = parsedRelease;

		// Release candidates (rc) and final builds (f) use the same set of numbers.
		if (type == 'c')
			type = 'f';

		if (!validUnityVersion)
			Debug.LogError("Encountered invalid Unity version string - Interpreted: " + unityVersion + " as: " + this.ToString());
	}

	public OVRUnityVersion(int major, int minor, int change, char type, int release)
    : this()
	{
		this.major = major;
		this.minor = minor;
		this.change = change;
		this.type = type;
		this.release = release;
	}

	public override int GetHashCode()
	{
		unchecked
		{
			int hash = 17;
			hash = hash * 23 + major.GetHashCode();
			hash = hash * 23 + minor.GetHashCode();
			hash = hash * 23 + change.GetHashCode();
			hash = hash * 23 + type.GetHashCode();
			hash = hash * 23 + release.GetHashCode();
			return hash;
		}
	}

	public override bool Equals(object obj)
	{
		if (obj == null)
			return false;

		if (obj is OVRUnityVersion)
			return this == (OVRUnityVersion)obj;

		if (obj is string)
			return this == (string)obj;

		return false;
	}

	// Compares two OVRUnityVersion instances in order of major, minor, change, type, and release versions.
	// Note - Desired 'type' ordering also matches alphabetical ordering, using that here for simplicity.
	// Ordered list of unity types:
	// 'a' = alpha release
	// 'b' = beta release
	// 'c' = internal release candidate
	// 'f' = public release candidate / public release
	// 'p' = patch release
	public int CompareTo(OVRUnityVersion other)
	{
		int majorCheck = this.major.CompareTo(other.major);
		if (majorCheck != 0)
			return majorCheck;

		int minorCheck = this.minor.CompareTo(other.minor);
		if (minorCheck != 0)
			return minorCheck;

		int changeCheck = this.change.CompareTo(other.change);
		if (changeCheck != 0)
			return changeCheck;

		int typeCheck = this.type.CompareTo(other.type);
		if (typeCheck != 0)
			return typeCheck;

		int releaseCheck = this.release.CompareTo(other.release);
		if (releaseCheck != 0)
			return releaseCheck;

		return 0;
	}

	public int CompareTo(string otherVersionString)
	{
		if (otherVersionString == null)
			return 1;

		return this.CompareTo(new OVRUnityVersion(otherVersionString));
	}

	public static bool operator  <(OVRUnityVersion v1, OVRUnityVersion v2) { return v1.CompareTo(v2)  < 0; }
	public static bool operator <=(OVRUnityVersion v1, OVRUnityVersion v2) { return v1.CompareTo(v2) <= 0; }
	public static bool operator  >(OVRUnityVersion v1, OVRUnityVersion v2) { return v1.CompareTo(v2)  > 0; }
	public static bool operator >=(OVRUnityVersion v1, OVRUnityVersion v2) { return v1.CompareTo(v2) >= 0; }
	public static bool operator ==(OVRUnityVersion v1, OVRUnityVersion v2) { return v1.CompareTo(v2) == 0; }
	public static bool operator !=(OVRUnityVersion v1, OVRUnityVersion v2) { return v1.CompareTo(v2) != 0; }
	public static bool operator  <(OVRUnityVersion v1, string v2) { return v1.CompareTo(v2)  < 0; }
	public static bool operator <=(OVRUnityVersion v1, string v2) { return v1.CompareTo(v2) <= 0; }
	public static bool operator  >(OVRUnityVersion v1, string v2) { return v1.CompareTo(v2)  > 0; }
	public static bool operator >=(OVRUnityVersion v1, string v2) { return v1.CompareTo(v2) >= 0; }
	public static bool operator ==(OVRUnityVersion v1, string v2) { return v1.CompareTo(v2) == 0; }
	public static bool operator !=(OVRUnityVersion v1, string v2) { return v1.CompareTo(v2) != 0; }

	public override string ToString()
	{
		return "" + major + "." + minor + "." + change + type + release;
	}
}

