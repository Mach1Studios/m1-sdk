using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.IO;

/// <summary>
/// http://forum.codecall.net/topic/60773-reading-ini-files-in-managed-code/
/// </summary>
namespace Utility
{

    /// <summary>
    /// Read-Only Support for INI Configuration Files
    /// </summary>
    public static class INIUtility : object
    {
        #region [Local Attributes]

        /// <summary>
        /// Line Comment Tokens
        /// </summary>
        private static string[] m_strLineComments = new string[] { "#", ";" };

        #endregion

        #region [Popular Interface]

        /// <summary>
        /// Read a single Value from an INI Configuration File
        /// </summary>
        /// <param name="strFileSource">File Location</param>
        /// <param name="strSection">Section Name</param>
        /// <param name="strKey">Key Name</param>
        /// <returns>Value of setting</returns>
        public static string Read(string strFileSource, string strSection, string strKey)
        {
            Dictionary<string, string> dictKeys = ReadAllSection(strFileSource, strSection);

            if (dictKeys.Keys.Contains(strKey))
                return dictKeys[strKey];
            else
                throw new Exception("Key does not exist");

        }

        /// <summary>
        /// Read an entire section
        /// </summary>
        /// <param name="strFileSource">File Location</param>
        /// <param name="strSection">Section Name</param>
        /// <returns>Dictionary, Containing Section Keys/Values</returns>
        public static Dictionary<string, string> ReadAllSection(string strFileSource, string strSection)
        {
            Dictionary<string, string> dictRet = new Dictionary<string, string>();

            //Grab Contents of section
            string strContents = GetSection(ReadFile(strFileSource), strSection);
            string[] strKeyPairs, strKeyValuePair;

            //Grab index of the section
            int intSectionPos = strContents.IndexOf("[" + strSection + "]");
            if (intSectionPos == -1)
                throw new Exception("Section strFileSource Found");

            strKeyPairs = strContents.Substring(("[" + strSection + "]").Length).Split(new string[] { "\r", "\n" }, StringSplitOptions.None);
            foreach (string strKeyPair in strKeyPairs)
            {
                strKeyValuePair = strKeyPair.Split(new string[] { "=" }, StringSplitOptions.RemoveEmptyEntries);
                if (strKeyValuePair.Length > 0)
                    if (strKeyValuePair.Length == 2)
                        dictRet[strKeyValuePair[0]] = strKeyValuePair[1];
                    else
                        dictRet[strKeyValuePair[0]] = string.Empty;
            }

            return dictRet;
        }

        /// <summary>
        /// Read Entire INI Configuration File
        /// </summary>
        /// <param name="strFileSource">File to Read</param>
        /// <returns>Collection containing all sections. and key/value pairs for each section</returns>
        public static Dictionary<string, Dictionary<string, string>> ReadAll(string strFileSource)
        {
            Dictionary<string, Dictionary<string, string>> dictValues = new Dictionary<string, Dictionary<string, string>>();
            Dictionary<string, string> dictKeyPairs;
            string strContents = ReadFile(strFileSource);

            foreach (string strSection in GetSections(strContents))
            {
                dictValues[strSection] = new Dictionary<string, string>();
                dictKeyPairs = ReadAllSection(strFileSource, strSection);
                foreach (string strKey in dictKeyPairs.Keys)
                {
                    dictValues[strSection][strKey] = dictKeyPairs[strKey];
                }
            }

            return dictValues;
        }

        #endregion

        #region [Helper Methods]

        /// <summary>
        /// Read the entire contents of the specified file (minus the comments)
        /// </summary>
        /// <param name="strFilePath">Path of file</param>
        /// <returns>Entire file contents in a string</returns>
        private static string ReadFile(string strFilePath)
        {
            StringBuilder sbInput = new StringBuilder();

            using (StreamReader sr = new StreamReader(strFilePath))
            {
                sbInput.Append(sr.ReadToEnd());
            }
            return CleanComments(sbInput.ToString()); //And clean the comments
        }

        /// <summary>
        /// Strips comments from INI Source
        /// </summary>
        /// <param name="strSource">Actual INI Configuration Contents</param>
        /// <returns>INI Configuration Contents (minus comments)</returns>
        private static string CleanComments(string strSource)
        {
            StringBuilder sbResponse = new StringBuilder();
            string[] strBuffer;
            //Propigate Line
            string[] strLines = strSource.Split(new string[] { "\r", "\n" }, StringSplitOptions.RemoveEmptyEntries);

            //Process Each Item in array
            foreach (string strLine in strLines)
            {
                if (!m_strLineComments.Contains(strLine.Trim()[0].ToString()))
                {
                    strBuffer = strLine.Split(m_strLineComments, StringSplitOptions.RemoveEmptyEntries);
                    if (strBuffer.Length > 1)
                    {
                        sbResponse.Append(strBuffer[0]);
                    }
                    else
                    {
                        sbResponse.Append(strLine);
                    }
                    sbResponse.Append("\r\n");
                }
            }

            return sbResponse.ToString();
        }

        /// <summary>
        /// Pulls INI Section (Header and Key/Value Pairs)
        /// </summary>
        /// <param name="strSource">Source Configuration Contents</param>
        /// <param name="strSection">Name of section to pull</param>
        /// <returns>Entire Section, Header and Key/Value Pairs</returns>
        private static string GetSection(string strSource, string strSection)
        {

            //Grab index of the section
            int intSectionPos = strSource.IndexOf("[" + strSection + "]");
            if (intSectionPos == -1)
                throw new Exception("Section Not Found");

            //We know the end of the section, either by another [, or by the EOF
            int intEndSectionPos = strSource.IndexOf("[", intSectionPos + 1);
            if (intEndSectionPos == -1)
                intEndSectionPos = strSource.Length;
            else
                intEndSectionPos -= 1;

            return strSource.Substring(intSectionPos, intEndSectionPos - intSectionPos);
        }

        /// <summary>
        /// Return the names of all sections in an INI configuration
        /// </summary>
        /// <param name="strSource">INI Configuration Contents</param>
        /// <returns>String array containing all section names</returns>
        private static string[] GetSections(string strSource)
        {
            int intLastPos = 0;
            int intEndSectPos;
            List<string> lstSections = new List<string>();

            //Ok, the idea is to keep looking for indexes of section beginnings, 
            intLastPos = strSource.IndexOf("[", intLastPos);
            while (intLastPos != -1)
            {
                intEndSectPos = strSource.IndexOf("]", intLastPos);

                lstSections.Add(strSource.Substring(intLastPos + 1, (intEndSectPos - 1) - (intLastPos)));

                intLastPos += 1;
                intLastPos = strSource.IndexOf("[", intLastPos);
            }

            return lstSections.ToArray();
        }

        #endregion

    }
}