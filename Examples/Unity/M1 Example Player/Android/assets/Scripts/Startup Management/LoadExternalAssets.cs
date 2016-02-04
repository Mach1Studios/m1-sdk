using UnityEngine;
using UnityEngine.UI;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using Utility;

public class LoadExternalAssets : MonoBehaviour
{
    public string folderName = "VRData";
    public string configFileName = "config.ini";
    
    Dictionary<string, string> mediaSettings;
    List<KeyMapping> items = new List<KeyMapping>();
    string path, logfile;
    public bool logging = false;
    public Text text;

    /// <summary>
    /// 1. enable loading screen
    /// 2. find key mappings
    /// 3. Check for the directory (create if does not exist and log warning)
    /// 4. Check for config file
    /// 5. process config file and load files
    /// </summary>
    void Awake()
    {
        // grab everything that has a key mapping
        items = FindObjectsOfType<KeyMapping>().ToList();

        // disable all items so that they don't try to load their data
        items.ForEach(x =>
        {
            x.gameObject.SetActive(false);
            if(logging)
            {
                Debug.Log("Found object: " + x.name);
            }
        });

        // Find the folder
        path = Application.persistentDataPath + "/" + folderName;
        logfile = path + "/" + "log.txt";
        print(logfile);
        if (!Directory.Exists(path))
        {
            DirectoryInfo di = Directory.CreateDirectory(path);
            Debug.LogWarning("Creating folder: " + path);
            File.AppendAllText(logfile, "Creating folder: " + path + "\n");
        }
        else
        {
            Debug.Log("Found folder: " + path);
            File.AppendAllText(logfile, "Found folder: " + path + "\n");
        }
        text.text = path;

        // check for config file
        string configFile = path + "/" + configFileName;
        if (File.Exists(configFile))
        {
            mediaSettings = INIUtility.ReadAllSection(path + "/" + configFileName, "Media");
            AssignStartUpMedia();
        }
        else
        {
            Debug.LogError("Config file not found: " + path);
            File.AppendAllText(logfile, "Config file not found: " + path + "\n");
        }
    }

    /// <summary>
    /// This method will assign the files we want to load
    /// </summary>
    void AssignStartUpMedia()
    {
        if (logging)
        {
            string map = "";
            foreach (KeyValuePair<string, string> entry in mediaSettings)
            {
                map += entry.Key + ":" + entry.Value + "\n";
            }
            Debug.Log("map: \n"+ map);
            File.AppendAllText(logfile, "map: \n" + map + "\n");
        }

        foreach (KeyMapping key in items)
        {
            if(!mediaSettings.ContainsKey(key.GetKeyName()))
            {
                Debug.LogWarning("Key: " + key.GetKeyName() + " not found.");
                File.AppendAllText(logfile, "Key: " + key.GetKeyName() + " not found.");
                continue;
            }
            string filePath = GetPath(mediaSettings[key.GetKeyName()]);

            DirectionalSound ds = key.GetComponent<DirectionalSound>();
            if(ds != null)
            {
                ds.audioPath = filePath;
            }

            MoviePlayerSample mps = key.GetComponent<MoviePlayerSample>();
            if(mps != null)
            {
                mps.moviePath = filePath;
            }

            key.gameObject.SetActive(true);
        }
    }

    string GetPath(string value)
    {
        switch(mediaSettings["type"])
        {
            case "web":
                return value;
            case "external":
                return "file:///" + value;
            case "local":
            default:
                return "file:///" + path + "/" + value;
        }

    }

}
