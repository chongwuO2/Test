
// RSsystemDoc.h : CRSsystemDoc ��Ľӿ�
//

#pragma once


class CRSsystemDoc : public CDocument
{
protected: // �������л�����
	CRSsystemDoc();
	DECLARE_DYNCREATE(CRSsystemDoc)

// ����
public:

// ����
public:

// ��д
public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
#ifdef SHARED_HANDLERS
	virtual void InitializeSearchContent();
	virtual void OnDrawThumbnail(CDC& dc, LPRECT lprcBounds);
#endif // SHARED_HANDLERS

// ʵ��
public:
	virtual ~CRSsystemDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// ���ɵ���Ϣӳ�亯��
protected:
	DECLARE_MESSAGE_MAP()
#ifdef SHARED_HANDLERS
	// ����Ϊ����������������������ݵ� Helper ����
	void SetSearchContent(const CString& value);
#endif // SHARED_HANDLERS
//	virtual BOOL OnOpenDocument(LPCTSTR lpszPathName);
//	virtual BOOL OnSaveDocument(LPCTSTR lpszPathName);
};
