using UnityEngine;
using System.Collections;
using UnityEngine.UI;
using UnityEngine.Events;
using UnityEngine.EventSystems;



#if !UNITY_WEBGL

public class SeekBarCtrl : MonoBehaviour ,IPointerDownHandler, IPointerUpHandler, IPointerEnterHandler, IPointerExitHandler, IDragHandler{

	public MediaPlayerCtrl m_srcVideo;
	public Slider m_srcSlider;
	public float m_fDragTime = 0.2f;


	bool m_bActiveDrag = true;
	bool m_bUpdate = true;

	float m_fDeltaTime = 0.0f;
	float m_fLastValue = 0.0f;
	float m_fLastSetValue = 0.0f;

	// Use this for initialization
	void Start () {
	
	}


	// Update is called once per frame
	void Update () {

		if (m_bActiveDrag == false) {
			m_fDeltaTime += Time.deltaTime;
			if (m_fDeltaTime > m_fDragTime) {
				m_bActiveDrag = true;
				m_fDeltaTime = 0.0f;
				//if(m_fLastSetValue != m_fLastValue)
				//	m_srcVideo.SetSeekBarValue (m_fLastValue);

			}
		}



		if (m_bUpdate == false)
			return;
			
		if (m_srcVideo != null) {

			if (m_srcSlider != null) {
				m_srcSlider.value = m_srcVideo.GetSeekBarValue();

			}
			
		}
	
	}

	public void OnPointerEnter(PointerEventData eventData)
	{
		Debug.Log("OnPointerEnter:");  

		m_bUpdate = false;



	}

	public void OnPointerExit(PointerEventData eventData)
	{
		Debug.Log("OnPointerExit:");

		m_bUpdate = true;


	}

	public void OnPointerDown(PointerEventData eventData)
	{


	}

	public void OnPointerUp(PointerEventData eventData)
	{
	
		m_srcVideo.SetSeekBarValue (m_srcSlider.value);






	}


	public void OnDrag(PointerEventData eventData)
	{
		 Debug.Log("OnDrag:"+ eventData);   

		if (m_bActiveDrag == false) 
		{
			m_fLastValue = m_srcSlider.value;
			return;
		}

		//m_srcVideo.SetSeekBarValue (m_srcSlider.value);
		m_fLastSetValue = m_srcSlider.value;
		m_bActiveDrag = false;
	
	}

}
#endif