using System;
using System.Collections.Generic;
using UnityEngine;

namespace assets.Scripts.Startup_Management
{
    public class StartupManager : MonoBehaviour
    {
        /// <summary>
        /// This holds references to the Sounds we want to load and then play in unison
        /// </summary>
        public List<DirectionalSound> Sounds = new List<DirectionalSound>();
        public List<MoviePlayerSample> Movies = new List<MoviePlayerSample>();
        public static Action OnLoadComplete;
        public GameObject LoadingScreen;

        public GameObject LoadingWheel;

        bool _bLoadComplete;

        public event EventHandler ScriptWasFinished;

        private DateTime _timer = DateTime.Now.AddSeconds(3);

        void Start()
        {
            // enable loading screen
            if (LoadingScreen != null) LoadingScreen.SetActive(true);

            if (LoadingWheel == null)
            {
                Debug.LogError("LoadingWheel not found");
            }
            else
            {
                LoadingWheel.SetActive(true);
            }

            if (Movies.Count == 0)
            {
                Debug.LogWarning("Error: No MoviePlayerSamples assigned");
            }

            if (Sounds.Count == 0)
            {
                Debug.LogWarning("Warning: No DirectionalSounds assigned");
            }
        }

        /// <summary>
        /// This method has 3 stages:
        /// 1. Wait until audio is complete loading
        /// 2. Once ready, play media in the same frame
        /// 3. After that, disable self
        /// </summary>
        void Update()
        {
            if (_timer > DateTime.Now)
            {
                return;
            }

            if (!_bLoadComplete)
            {
                CheckAudioReady();
                LoadingWheel.SetActive(false);
            }
            else
            {
                for (int i = 0; i < Movies.Count; i++)
                {
                   // Movies[i].PlayVideo();
                }
                for (int i = 0; i < Sounds.Count; i++)
                {
                    Sounds[i].PlayAudio();
                }

                OnScriptWasFinished();
                gameObject.SetActive(false);
            }
        }

        /// <summary>
        /// Query our directional Sounds to see if the audio was loaded
        /// </summary>
        void CheckAudioReady()
        {
            bool bReady = true;
            for (int i = 0; i < Sounds.Count; i++)
            {
                if (!Sounds[i].IsAudioLoaded)
                {
                    bReady = false;
                }
            }

            if (bReady)
            {
                if (OnLoadComplete != null)
                {
                    OnLoadComplete();
                }
                _bLoadComplete = true;
                if (LoadingScreen != null) LoadingScreen.SetActive(false);
            }
        }

        protected virtual void OnScriptWasFinished()
        {
            var handler = ScriptWasFinished;
            if (handler != null) handler(this, EventArgs.Empty);
        }
    }
}
