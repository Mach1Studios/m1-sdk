using assets.Scripts.ButtonsScripts;
using UnityEngine;

namespace assets.Scripts.Utility
{
    public class TransportControlButtonTest:MonoBehaviour
    {
        public void StartStop(VideoPlayerButtonEventType eventTypes)
        {
            if (eventTypes == VideoPlayerButtonEventType.Start)
            {
                DiagnosticMessage.Print("Start button pressed");
            }

            if (eventTypes == VideoPlayerButtonEventType.Stop)
            {
                DiagnosticMessage.Print("Stop button pressed");
            }
        }

        public void Forward(VideoPlayerButtonEventType eventTypes)
        {
            DiagnosticMessage.Print("Forward button pressed");
        }

        public void Back(VideoPlayerButtonEventType eventTypes)
        {
            DiagnosticMessage.Print("Back button pressed");
        }
    }
}