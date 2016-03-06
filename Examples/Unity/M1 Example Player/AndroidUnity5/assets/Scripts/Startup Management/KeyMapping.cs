using UnityEngine;

public class KeyMapping : MonoBehaviour
{
    [Header("If left empty, this defaults to the object name")]
    public string keyName;

    public string GetKeyName()
    {
        if (string.IsNullOrEmpty(keyName))
        {
            return name;
        }

        return keyName;
    }
}
