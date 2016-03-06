using UnityEngine;

namespace assets.Scripts
{
    public class LoadingWheelController : MonoBehaviour
    {
        public float RotationSpeed = 400;

        // Use this for initialization
        void Start()
        {

        }

        // Update is called once per frame
        void Update()
        {
            transform.Rotate(-Vector3.forward, RotationSpeed * Time.deltaTime);
        }
    }
}
