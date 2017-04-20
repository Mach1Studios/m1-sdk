using UnityEngine;
using UnityEditor;

using System.Xml;

public class CSProjectFilePostProcessor : AssetPostprocessor
{
    // Add unsafe option to generated project file for Third Party assets that require it such as Easy Movie Texture.
    static void OnGeneratedCSProjectFiles()
    {

        if (System.IO.File.Exists("Assembly-CSharp.csproj"))
        {
            XmlDocument document = new XmlDocument();
            document.Load("Assembly-CSharp.csproj");
            XmlNodeList nodes = document.SelectNodes("//*[contains(@Condition, 'AnyCPU')]");

            foreach (XmlNode node in nodes)
            {
                if (node.Name == "PropertyGroup")
                {
                    XmlElement element = document.CreateElement("AllowUnsafeBlocks", node.NamespaceURI);
                    element.InnerText = "true";
                    node.AppendChild(element);
                }
            }

            document.Save("Assembly-CSharp.csproj");
        }
        
        
    }
}