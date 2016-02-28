using UnityEngine;

namespace assets.Scripts.Input_Management
{
    public class MouseController : MonoBehaviour
    {
        public float CameraHorizontalSpeed = 50f;
        public float CameraVerticalSpeed = 50f;
        //public float MaxVerticalAngle = 45;
        //public float MinVerticalAngle = 10;

        // Use this for initialization
        void Start()
        {

        }

        // Update is called once per frame
        void Update()
        {

#if UNITY_EDITOR

            //horizontal movement
            var horizontalAngle = Input.GetAxis("Mouse X") * CameraHorizontalSpeed * Time.deltaTime;
            transform.Rotate(Vector3.up, horizontalAngle, Space.World);

            //vertical movement
            var verticalAngle = Input.GetAxis("Mouse Y") * CameraVerticalSpeed * Time.deltaTime;
            //var currentVerticalAngle = transform.eulerAngles.x;
            //if (currentVerticalAngle + verticalAngle > MaxVerticalAngle)
            //{
            //    verticalAngle = MaxVerticalAngle - currentVerticalAngle;
            //}

            //if (currentVerticalAngle + verticalAngle < MinVerticalAngle)
            //{
            //    verticalAngle = MinVerticalAngle - currentVerticalAngle;
            //}

            transform.Rotate(Vector3.right, -verticalAngle, Space.Self);
#endif
        }
    }
}