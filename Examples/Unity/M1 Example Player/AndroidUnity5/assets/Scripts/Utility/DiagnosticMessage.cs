using UnityEngine;
using UnityEngine.UI;

namespace assets.Scripts.Utility
{
    public class DiagnosticMessage : MonoBehaviour
    {
        private static GameObject _diagnosticTextGameObject;

        // Use this for initialization
        void Start()
        {
            _diagnosticTextGameObject = gameObject;
        }

        // Update is called once per frame
        void Update()
        {

        }

        public static void Print(string message, params object[] obj)
        {
            var diagnosticMessage = string.Format(message, obj);

            if (_diagnosticTextGameObject != null && _diagnosticTextGameObject.GetComponent<Text>() != null)
            {
                _diagnosticTextGameObject.GetComponent<Text>().text = diagnosticMessage;
            }
            print(diagnosticMessage);
        }
    }
}
