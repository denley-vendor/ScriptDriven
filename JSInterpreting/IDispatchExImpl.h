//
// IDispatchExImpl.h
//
// --------------------------------------------------------
// A Practical Guide to Script-Driven Software Development
// Author: Qiming Lu                        Date: 6/1/2006
// MSN Messager: luqiming26@hotmail.com
// MSN Blog: http://spaces.msn.com/jemylu
// --------------------------------------------------------

#ifndef __IDispatchExImpl_H__
#define __IDispatchExImpl_H__

#include "IDispatchBaseImpl.h"
#include <map>

// Determine if the user of this class has specified a start value for DISPIDs
#ifndef DISPID_START
#define DISPID_START 0
#endif

class CDynamicDispatchEntry;

template<class DERIVED_CLASS, class BASE_ITF, const IID* BASE_IID>
class __declspec(novtable) IDispatchExImpl : public IDispatchBaseImpl<DERIVED_CLASS, BASE_ITF, BASE_IID>
{
protected:
	// For looking up information
	class BSTRGreater
	{
	public:
		bool operator() (const BSTR str1, const BSTR str2) const
		{
			if (wcscmp(str1, str2) > 0)
				return true;
			else
				return false;
		}
	};

	// typedefs for easier reading
	typedef IDispatchBaseImpl<DERIVED_CLASS, BASE_ITF, BASE_IID> __base;
	typedef std::map<BSTR, CDynamicDispatchEntry*, BSTRGreater>	MapNameToEntry;
	typedef std::map<DISPID, CDynamicDispatchEntry*> MapDispidToEntry;
	
	MapDispidToEntry	m_mapDispidToEntry;
	MapNameToEntry		m_mapNameToEntry;
	MapNameToEntry		m_mapCaselessNameToEntry; // For caseless lookups
	DISPID				m_dispidNext; // The current dispid to add entries at

	// For multi-threaded safety
//	ThreadSafeRefCount	m_csAccessSync;
	

	// Helpers

	/***************************************************************************
	* Function:	IDispatchExImpl::GetEntryByName()
	* Args:		<BSTR> bstrName - the name of the entry to find
	*			<DWORD> dwGrfDex - the flags describing the comparison operator
	* Returns:	<CDynamicDispatchEntry*> the entry that was found or NULL
	* Purpose:	Finds an entry by name and case sensitivity
	***************************************************************************/
	CDynamicDispatchEntry* GetEntryByName(BSTR bstrName,
		DWORD dwGrfDex = fdexNameCaseInsensitive)
	{
		CDynamicDispatchEntry* pEntry = 0;

		// Determine the type of comparison to use
		if (dwGrfDex & fdexNameCaseSensitive)
		{
			// Get the pointer to the entry
			MapNameToEntry::const_iterator it = m_mapNameToEntry.find(bstrName);
			if (it != m_mapNameToEntry.end()) 
			{
				pEntry = (*it).second;
			}
		}
		else
		{
			// Make a copy of the string
			BSTR strTemp = SysAllocString(bstrName);
			// Convert to a consistent case
			_wcslwr(strTemp);
			// Get the pointer to the entry
			MapNameToEntry::const_iterator it = m_mapCaselessNameToEntry.find(strTemp);
			if (it != m_mapCaselessNameToEntry.end()) 
			{
				pEntry = (*it).second;
			}
		}
		return pEntry;
	}

	/***************************************************************************
	* Function:	IDispatchExImpl::CreateNewEntry()
	* Args:		<BSTR> bstrName - the name of the entry to find
	*			<DISPID*> pDispID - the out parameter of the dispid
	*			<VARIANT> vtVal - the starting value of the property
	*			<bool> bTypeInfo - whether this method is contained in the
	*			type info or our map
	*			<bool> bUseInDispid - forces the use of the passed in dispid
	* Returns:	<HRESULT> Standard COM codes
	* Purpose:	Creates a new dispatch entry
	***************************************************************************/
	HRESULT CreateNewEntry(BSTR bstrName, DISPID* pDispID, VARIANT vtVal,
		bool bTypeInfo = false, bool bUseInDispid = false)
	{
		HRESULT hr = E_OUTOFMEMORY;
		
		// Determine if the passed in dispid should be used
		if (bUseInDispid == true)
		{
			// Copy the dispid
			m_dispidNext = *pDispID;
		}

		// Try to allocate a new entry
		CDynamicDispatchEntry* pEntry = 0;
		pEntry = new CDynamicDispatchEntry(bstrName, m_dispidNext, vtVal, bTypeInfo);
		if (pEntry)
		{
			// Add the new entry to the dispid map
			m_mapDispidToEntry[pEntry->GetDispID()] = pEntry;
			// Add it to the case sensitive map
			BSTR strName1 = SysAllocString(bstrName);
			m_mapNameToEntry[strName1] = pEntry;
			// Add it to the caseless map
			BSTR strName2 = SysAllocString(bstrName);
			_wcslwr(strName2);
			m_mapCaselessNameToEntry[strName2]= pEntry;

			// Copy the outbound dispid
			*pDispID = pEntry->GetDispID();
			// Increment the next dispid. This is done here to prevent holes
			// in the dispid range on failed creates
			m_dispidNext = (m_dispidNext < 0) ? 0 : m_dispidNext + 1;
			// Tell the caller that everything worked
			hr = S_OK;
		}
		return hr;
	}

	/***************************************************************************
	* Function:	IDispatchExImpl::GetStartDispID()
	* Args:		<DISPID*> pDispID - the out parameter that gets the dispid
	* Returns:	<HRESULT> Standard COM codes
	* Purpose:	Returns the first dispid in the map. This will look for the first
	*			not deleted entry and return that dispid.
	***************************************************************************/
	HRESULT GetStartDispID(DISPID* pDispID)
	{
		// Default to no entries available
		HRESULT hr = S_FALSE;

		bool bDone = false;
		MapDispidToEntry::iterator it, itEnd(m_mapDispidToEntry.end());
		for (it = m_mapDispidToEntry.begin(); it != itEnd && !bDone; ++it)
		{
			DISPID dispid = -1;

			// Determine if this item is not deleted
			CDynamicDispatchEntry* pEntry = (*it).second;
			if (pEntry->IsDeleted() == false)
			{
				// No need to look further
				bDone = true;
				// Return the dispid
				*pDispID = dispid;
				// Change the HRESULT to S_OK so that the caller does not
				// think that we have no members
				hr = S_OK;
			}
		}
		return hr;
	}

	/***************************************************************************
	* Function:	IDispatchExImpl::GetNextDispID()
	* Args:		<DISPID> dispid - the current dispid to search from
	*			<DISPID*> pDispID - the out parameter that gets the dispid
	* Returns:	<HRESULT> Standard COM codes
	* Purpose:	Returns the nextt dispid in the map. This will look for the first
	*			not deleted entry and return that dispid.
	***************************************************************************/
	HRESULT GetNextDispID(DISPID dispid, DISPID* pDispID)
	{
		// Default to no entries available
		HRESULT hr = S_FALSE;

		// Search until we have no more entries or found the next item
		DISPID curDispid = dispid;
		bool bDone = false;
		while (!bDone)
		{
			// Move to what we think is the next dispid
			curDispid++;

			MapDispidToEntry::const_iterator it = m_mapDispidToEntry.find(curDispid);
			if (it != m_mapDispidToEntry.end()) 
			{
				CDynamicDispatchEntry* const pEntry = (*it).second;
				// Determine if this item is not deleted
				if (pEntry->IsDeleted() == false)
				{
					// No need to look further
					bDone = true;
					// Return the dispid
					*pDispID = curDispid;
					// Change the HRESULT to S_OK so that the caller does not
					// think that we have no members
					hr = S_OK;
				}
			}
			else
			{
				bDone = true;
			}
		}
		return hr;
	}

	/***************************************************************************
	* Function:	IDispatchExImpl::GetVariantByDispID()
	* Args:		<DISPID> dispid - the current dispid to search for
	*			<VARIANT*> pvtResult - the variant that holds the out value
	* Returns:	<HRESULT> Standard COM codes
	* Purpose:	Copies the variant held at the specified dispid
	***************************************************************************/
	HRESULT GetVariantByDispID(DISPID dispid, VARIANT* pvtResult)
	{
		// Clear the destination variant
		HRESULT hr = VariantClear(pvtResult);
		if (SUCCEEDED(hr))
		{
			// Look for the entry corresponding to this dispatch
			MapDispidToEntry::const_iterator it = m_mapDispidToEntry.find(dispid);
			if (it != m_mapDispidToEntry.end()) 
			{
				CDynamicDispatchEntry* const pEntry = (*it).second;
				// Only do the copy if this is a dynamic property
				if (pEntry->UseTypeInfo() == false)
				{
					// Copy the VARIANT from the entry
					hr = pEntry->GetVar(pvtResult);
				}
				else
				{
					// Tell the caller that this dispid is TypeInfo based
					hr = S_FALSE;
				}
			}
			else
			{
				// Unable to find the specified entry
				hr = DISP_E_MEMBERNOTFOUND;
			}			
		}
		return hr;
	}

	/***************************************************************************
	* Function:	IDispatchExImpl::SetVariantByDispID()
	* Args:		<DISPID> dispid - the current dispid to search for
	*			<VARIANT*> pvtResult - the variant that we are to copy
	* Returns:	<HRESULT> Standard COM codes
	* Purpose:	Copies the variant passed in to the entry mapped to the dispid
	***************************************************************************/
	HRESULT SetVariantByDispID(DISPID dispid, VARIANT* pvtResult)
	{
		// Default to unable to find the specified entry
		HRESULT hr = DISP_E_MEMBERNOTFOUND;

		// Look for the entry corresponding to this dispatch
		MapDispidToEntry::const_iterator it = m_mapDispidToEntry.find(dispid);
		if (it != m_mapDispidToEntry.end()) 
		{
			CDynamicDispatchEntry* const pEntry = (*it).second;
			// Only do the copy if this is a dynamic property
			if (pEntry->UseTypeInfo() == false)
			{
				// Set the VARIANT in the entry
				hr = pEntry->SetVar(pvtResult);
			}
			else
			{
				// Tell the caller that this dispid is TypeInfo based
				hr = S_FALSE;
			}
		}
		return hr;
	}

	/***************************************************************************
	* Function:	IDispatchExImpl::AddTypeInfoDispIDs()
	* Args:		None
	* Returns:	<HRESULT> Standard COM codes
	* Purpose:	Uses the TypeInfo interface to add any members that the derived
	*			class needs to expose.
	***************************************************************************/
	HRESULT AddTypeInfoDispIDs(void)
	{
		// Determine the number of methods in the object
		TYPEATTR* pta = 0;
		HRESULT hr = m_pti->GetTypeAttr(&pta);
		if (SUCCEEDED(hr))
		{
			// Loop through getting the information on each member
			for (UINT index = 0; index < pta->cFuncs && SUCCEEDED(hr); ++index)
			{
				// Get the function description for this member
				FUNCDESC* pfd = 0;
				hr = m_pti->GetFuncDesc(index, &pfd);
				if (SUCCEEDED(hr))
				{
					BSTR bstrName = 0;
					// Determine the name of this member
					hr = m_pti->GetDocumentation(pfd->memid, &bstrName, 0, 0, 0);
					if (SUCCEEDED(hr))
					{
						// Look to see if we have already added an entry for
						// the dispid. This happens since get/put properties
						// share the same id.
						MapDispidToEntry::const_iterator it = m_mapDispidToEntry.find(pfd->memid);
						if (it == m_mapDispidToEntry.end()) 
						{
							// Add the member to our dispatch maps
							hr = CreateNewEntry(bstrName, &pfd->memid, VARIANT(), true, true);
						}
					}

					// Release the memory alloced on our behalf
					if (bstrName)
						SysFreeString(bstrName);
					m_pti->ReleaseFuncDesc(pfd);
				}
			}

			// Release the memory alloced on our behalf
			m_pti->ReleaseTypeAttr(pta);
		}
		return hr;
	}

	virtual HRESULT OnTypeInfoLoaded() 
	{
		// We need to determine which dispids the interface already
		// uses to prevent collisions when dynamic properties are
		// added to the object
		AddTypeInfoDispIDs();
		return S_OK;
	}

	/***************************************************************************
	* Function:	IDispatchExImpl::InvokeByDispID()
	* Args:		<DISPID> dispid - the dispid of the entry to invoke
	*			<LCID> lcid - the locale id
	*			<WORD> wFlags - calling flags
	*			<DISPPARAMS*> pDispParams - the dispparams of the method
	*			<VARIANT*> pvtResult - the variant holding the result
	*			<EXCEPINFO*> pExcepInfo - the exception info pointer
	* Returns:	<HRESULT> Standard COM codes
	* Purpose:	Invokes the LPDISPATCH held in the dispid passed in
	***************************************************************************/
	HRESULT InvokeByDispID(DISPID dispid, LCID lcid, WORD wFlags,
		DISPPARAMS* pDispParams, VARIANT* pvtResult, EXCEPINFO* pExcepInfo)
	{
		HRESULT hr = DISP_E_MEMBERNOTFOUND;

		// Look up the entry in the dispid map
		MapDispidToEntry::const_iterator it = m_mapDispidToEntry.find(dispid);
		if (it != m_mapDispidToEntry.end()) 
		{
			CDynamicDispatchEntry* const pEntry = (*it).second;
			
			// Get the variant that holds the dispatch
			VARIANT		vtDisp;
			hr = pEntry->GetVar(&vtDisp);
			if (SUCCEEDED(hr))
			{
				// Check to see that the item is not deleted
				if (pEntry->IsDeleted() == false)
				{
					UINT nErrArg = 0;
					// Determine if this is an exposed method or a
					// dynamically added property/method
					if (pEntry->UseTypeInfo() == false)
					{
						// Call the Zeroth member of the dispatch object
						// This should be the only function contained in
						// the dispatch object
						hr = vtDisp.pdispVal->Invoke(0,IID_NULL,lcid,wFlags,
							pDispParams,pvtResult,pExcepInfo,&nErrArg);
					}
					else
					{
						// This should be forwarded to the TypeInfo
						hr = S_FALSE;
					}
				}
				else
				{
					// Can't invoke one that was deleted
					hr = DISP_E_MEMBERNOTFOUND;
				}

				// Clear the variable
				VariantClear(&vtDisp);
			}
		}	
		return hr;
	}

public:
	
	// These two methods allow for access to the members via name
	// Add entries into your IDL if you want this functionality

	/***************************************************************************
	* Function:	IDispatchExImpl::Get()
	* Args:		<BSTR> bstrVarName - the name of the variable to fetch
	*			<VARIANT*> pvtVar - the object that will get the data
	*			<DWORD> dwGrfDex - the flags describing the comparison operator
	* Returns:	<HRESULT> Standard COM codes
	* Purpose:	Retrieves the variable by name. This is an optional method of
	*			getting properties.
	***************************************************************************/
	STDMETHOD(Get)(BSTR bstrVarName, VARIANT* pvtVar, 
		DWORD dwGrfDex = fdexNameCaseSensitive)
	{
		// Serialize access, since another thread might try to modify members at
		// the same time we are accessing them
	//	RefCountExclusiveLock lock(m_csAccessSync);

		HRESULT hr = S_OK;
		// Get the entry of the specified named var
		CDynamicDispatchEntry* pEntry = GetEntryByName(bstrVarName, dwGrfDex);
		if (pEntry)
		{
			// Check to see if this is an ITypeInfo member
			if (pEntry->UseTypeInfo() == false)
			{
				// Copy the variant
				hr = pEntry->GetVar(pvtVar);
			}
			else
			{
				// Build the params that we want to use
				DISPPARAMS dispparams = { NULL, NULL, 0, 0 };
				// Forward to the type info
				hr = m_pti->Invoke(this, pEntry->GetDispID(), DISPATCH_PROPERTYGET | DISPATCH_METHOD,
					&dispparams, pvtVar, 0, 0);
			}
		}
		else
		{
			// Clear the variable
			hr = VariantClear(pvtVar);
		}
		return hr;
	}

	/***************************************************************************
	* Function:	IDispatchExImpl::Set()
	* Args:		<BSTR> bstrVarName - the name of the variable to fetch
	*			<VARIANT*> pvtVar - the object that will give the data
	*			<BOOL> bAutoAdd - if the variable not found, add it automatically
	*			<DWORD> dwGrfDex - the flags describing the comparison operator
	* Returns:	<HRESULT> Standard COM codes
	* Purpose:	Sets the variable by name. This is an optional method of
	*			setting properties. NOTE: This must be declared as propertyput
	*			in the IDL file or the typeinfo won't process it.
	***************************************************************************/
	STDMETHOD(Set)(BSTR bstrVarName, VARIANT* pvtVar, BOOL bAutoAdd = TRUE,
		DWORD dwGrfDex = fdexNameCaseSensitive)
	{
		// Serialize access, since another thread might try to modify members at
		// the same time we are accessing them
	//	RefCountExclusiveLock lock(m_csAccessSync);

		HRESULT hr = E_INVALIDARG;
		// Get the entry of the specified named var
		CDynamicDispatchEntry* pEntry = GetEntryByName(bstrVarName, dwGrfDex);
		if (pEntry)
		{
			// Check to see if this is an ITypeInfo member
			if (pEntry->UseTypeInfo() == false)
			{
				// Copy the variant
				hr = pEntry->SetVar(pvtVar);
			}
			else
			{
				// Named var of type put
				DISPID dispid = DISPID_PROPERTYPUT;
				// Build the params that we want to use
				DISPPARAMS dispparams = { pvtVar, &dispid, 1, 1 };
				// Forward to the type info
				hr = m_pti->Invoke(this, pEntry->GetDispID(), DISPATCH_PROPERTYPUT | DISPATCH_PROPERTYPUTREF,
					&dispparams, 0, 0, 0);
			}
		}
		else
		{
			// The entry was not found. We may create it here!
			if (bAutoAdd)
			{
				DISPID dispid = 0;
				hr = CreateNewEntry(bstrVarName, &dispid, *pvtVar);
			}
		}
		return hr;
	}

public:
	IDispatchExImpl() : m_dispidNext(DISPID_START)
	{
	}

	virtual ~IDispatchExImpl()
	{
		// Need to iterate through the dispid map and delete the Entry pointers
		MapNameToEntry::iterator it, itEnd(m_mapNameToEntry.end());
		for (it = m_mapNameToEntry.begin(); it != itEnd; ++it)
		{
			if ((*it).first)  // BSTR
			{
				SysFreeString((*it).first);
			}
			if ((*it).second) // Entry pointer
			{
				delete (*it).second;
			}
		}
		for (it = m_mapCaselessNameToEntry.begin(); it != m_mapCaselessNameToEntry.end(); ++it)
		{
			if ((*it).first)  // BSTR
			{
				SysFreeString((*it).first);
			}
		}
	}


	// --- IUnknown methods --- 

	HRESULT STDMETHODCALLTYPE QueryInterface(REFIID riid,
		void **ppvObject) 
	{
		if (IID_IDispatchEx == riid)
		{
			*ppvObject = static_cast<IDispatchEx*> (this);
			AddRef();
			return S_OK;
		}
		else
		{
			return __base::QueryInterface(riid, ppvObject);
		}
	}


	// --- IDispatch methods ---

	/***************************************************************************
	* Function:	IDispatchExImpl::GetTypeInfoCount()
	* Args:		<UINT*> pnTypeInfoCount - always set to one
	* Returns:	<HRESULT> Standard COM codes
	* Purpose:	Tells the caller the number of type info interfaces that we
	*			support.
	***************************************************************************/
	STDMETHOD(GetTypeInfoCount)(UINT* pnTypeInfoCount)
	{
		// This call is inherently thread safe so no synchronization is needed
		// Validate the pointer passed in

		// We only return type info for the derived class
		*pnTypeInfoCount = 1;
		return S_OK;
	}

	/***************************************************************************
	* Function:	IDispatchExImpl::GetTypeInfo()
	* Args:		<UINT> nTypeInfoIndex - must be 1
	*			<LCID> ignored
	*			<ITypeInfo**> ppTypeInfo - the pointer that gets a copy of ours
	* Returns:	<HRESULT> Standard COM codes
	* Purpose:	This method returns a pointer to the derived class' type info
	***************************************************************************/
	STDMETHOD(GetTypeInfo)(UINT nTypeInfoIndex, LCID, ITypeInfo** ppTypeInfo)
	{
		// This call is inherently thread safe so no synchronization is needed
		// Validate the pointer passed in

		nTypeInfoIndex;

		*ppTypeInfo = m_pti;
		m_pti->AddRef();
		return S_OK;
	}

	/***************************************************************************
	* Function:	IDispatchExImpl::GetIDsOfNames()
	* Args:		<REFIID> reserved, so ignored
	*			<LPOLESTR*> szNameArray - the array of names to map to DISPIDs
	*			<UINT> nNames - the number of names in the array
	*			<LCID> lcid - the locale indicator to look up the names for
	*			<DISPID*> parrDispIDs - the array of DISPIDs to fill in
	* Returns:	<HRESULT> Standard COM codes
	* Purpose:	Determines the DISPID of each named property or method.
	***************************************************************************/

	STDMETHOD(GetIDsOfNames)(REFIID, LPOLESTR* szNameArray, UINT nNames, LCID,
		DISPID* parrDispIDs)
	{
		// Serialize access, since another thread might try to modify members at
		// the same time
	//	RefCountExclusiveLock lock(m_csAccessSync);

		HRESULT hr = S_OK;
		// Loop through the arrays letting GetDispID do the work
		for (UINT index = 0; index < nNames && SUCCEEDED(hr); ++index)
		{
			// Pass the call on to GetDispID
			// Since the caller is using IDispatch to get the ID and
			// not the Ex version to get it, assume case insensitivity
			hr = GetDispID(szNameArray[index], fdexNameCaseInsensitive,	&parrDispIDs[index]);
		}
		return hr;
	}

	/***************************************************************************
	* Function:	IDispatchExImpl::Invoke()
	* Args:		<DISPID> dispid - the dispid of the entry to invoke
	*			<REFIID> ignored
	*			<LCID> lcid - the locale id
	*			<WORD> wFlags - calling flags
	*			<DISPPARAMS*> pDispParams - the dispparams of the method
	*			<VARIANT*> pvtResult - the variant holding the result
	*			<EXCEPINFO*> pExcepInfo - the exception info pointer
	*			<UINT*> pnArgErr - the number of arguments that had errors
	* Returns:	<HRESULT> Standard COM codes
	* Purpose:	Calls the method of a dispatch id
	***************************************************************************/
	STDMETHOD(Invoke)(DISPID dispIdMember, REFIID, LCID lcid, WORD wFlags,
		DISPPARAMS* pDispParams, VARIANT* pVarResult, EXCEPINFO* pExcepInfo,
		UINT* pnArgErr)
	{
		// Serialize access, since another thread might try to modify members at
		// the same time
	//	RefCountExclusiveLock lock(m_csAccessSync);

		if (pnArgErr)
		{
			// Zero it out
			*pnArgErr = 0;
		}
		return InvokeEx(dispIdMember, lcid, wFlags, pDispParams, pVarResult, pExcepInfo, 0);
	}


	// --- IDispatchEx methods ---

	/***************************************************************************
	* Function:	IDispatchExImpl::DeleteMemberByDispID()
	* Args:		<DISPID> dispid - the dispid to delete from the dynamic props
	* Returns:	<HRESULT> Standard COM codes
	* Purpose:	Deletes a property/method from the dynamic list
	***************************************************************************/
	STDMETHOD(DeleteMemberByDispID)(DISPID dispid)
	{
		// Serialize access, since another thread might try to modify members at
		// the same time
	//	RefCountExclusiveLock lock(m_csAccessSync);

		HRESULT hr = S_OK;
		// Get the pointer to the entry
		MapDispidToEntry::const_iterator it = m_mapDispidToEntry.find(dispid);
		if (it != m_mapDispidToEntry.end()) 
		{
			CDynamicDispatchEntry* const pEntry = (*it).second;
			hr = pEntry->MarkDeleted();
		}
		return hr;
	}

	/***************************************************************************
	* Function:	IDispatchExImpl::DeleteMemberByName()
	* Args:		<BSTR> bstrName - the name of the entry to delete
	*			<DWORD> dwGrfDex - the flags describing the comparison operator
	* Returns:	<HRESULT> Standard COM codes
	* Purpose:	Deletes a property/method from the dynamic list
	***************************************************************************/
	STDMETHOD(DeleteMemberByName)(BSTR bstrName, DWORD dwGrfDex)
	{
		// Serialize access, since another thread might try to modify members at
		// the same time
	//	RefCountExclusiveLock lock(m_csAccessSync);

		HRESULT hr = S_OK;
		// Get a pointer to the entry if valid
		CDynamicDispatchEntry* pEntry = GetEntryByName(bstrName, dwGrfDex);
		if (pEntry)
		{
			hr = pEntry->MarkDeleted();
		}
		return hr;
	}

	/***************************************************************************
	* Function:	IDispatchExImpl::GetDispID()
	* Args:		<BSTR> bstrName - the name of the entry to get the dispid of
	*			<DWORD> dwGrfDex - the flags describing the comparison operator
	*			<DISPID*> pDispID - the out param of the dispid
	* Returns:	<HRESULT> Standard COM codes
	* Purpose:	Returns the DISPID of a given named entry
	***************************************************************************/
	STDMETHOD(GetDispID)(BSTR bstrName, DWORD dwGrfDex, DISPID* pDispID)
	{
		// Serialize access, since another thread might try to modify members at
		// the same time
	//	RefCountExclusiveLock lock(m_csAccessSync);

		HRESULT hr = S_OK;
		// Find the entry that corresponds to this name
		CDynamicDispatchEntry* pEntry = GetEntryByName(bstrName, dwGrfDex);
		if (pEntry)
		{
			// Check to see if this item was deleted or if it should be
			// added if it was
			if (pEntry->IsDeleted() == false ||
				(pEntry->IsDeleted() == true && dwGrfDex & fdexNameEnsure))
			{
				// Set the out parameter to this entry's dispid
				*pDispID = pEntry->GetDispID();
			}
			else
			{
				// Tell the caller that the member was deleted
				hr = DISP_E_UNKNOWNNAME;
			}
		}
		else
		{
			// The entry was not found, so we need to check if we are
			// supposed to automatically add it
			if (dwGrfDex & fdexNameEnsure)
			{
				// We need to create a new entry
				hr = CreateNewEntry(bstrName, pDispID, VARIANT());
			}
			else
			{
				// The item was not found
				hr = DISP_E_UNKNOWNNAME;
			}
		}
	
		// If can't find the dispid set the outgoing var
		if (FAILED(hr))
		{
			// Set the dispid to Unknown
			*pDispID = DISPID_UNKNOWN;
		}
		return hr;
	}

	/***************************************************************************
	* Function:	IDispatchExImpl::GetMemberName()
	* Args:		<DISPID> dispid - the dispid to get the name of
	*			<BSTR*> pbstrName - receives the name of the entry
	* Returns:	<HRESULT> Standard COM codes
	* Purpose:	Returns the name of a given dispid entry
	***************************************************************************/
	STDMETHOD(GetMemberName)(DISPID dispid, BSTR* pbstrName)
	{
		// Serialize access, since another thread might try to modify members at
		// the same time
	//	RefCountExclusiveLock lock(m_csAccessSync);

		HRESULT hr = S_OK;
		// Look up the entry in the dispid map
		// to find the entry that corresponds to this name
		MapDispidToEntry::const_iterator it = m_mapDispidToEntry.find(dispid);
		if (it != m_mapDispidToEntry.end()) 
		{
			CDynamicDispatchEntry* const pEntry = (*it).second;
			// Make sure this was not deleted
			if (pEntry->IsDeleted() == false)
			{
				// Copy the name of the entry
				*pbstrName = SysAllocString(pEntry->GetName());
			}
			else
			{
				// Indicate that we do not know what is going on
				hr = DISP_E_MEMBERNOTFOUND;
			}
		}
		else
		{
			// Indicate that we do not know what is going on
			hr = DISP_E_MEMBERNOTFOUND;
		}
		return hr;
	}

	/***************************************************************************
	* Function:	IDispatchExImpl::GetMemberProperties()
	* Args:		<DISPID> dispid - the dispid to get the name of
	*			<DWORD> dwGrfDex - the fetch flags (ignored)
	*			<BSTR*> pbstrName - receives the name of the entry
	* Returns:	<HRESULT> Standard COM codes
	* Purpose:	Returns the name of a given dispid entry
	***************************************************************************/
	STDMETHOD(GetMemberProperties)(DISPID dispid, DWORD, DWORD* pdwGrfDex)
	{
		// Serialize access, since another thread might try to modify members at
		// the same time
	//	RefCountExclusiveLock lock(m_csAccessSync);

		dispid;
		if (pdwGrfDex)
		{
			// Set the flags to "can do everything"
			*pdwGrfDex = grfdexPropCanAll;
		}
		return S_OK;
	}

	/***************************************************************************
	* Function:	IDispatchExImpl::GetNameSpaceParent()
	* Args:		<IUnknown**> ignored
	* Returns:	<HRESULT> Standard COM codes
	* Purpose:	Not supported
	***************************************************************************/
	STDMETHOD(GetNameSpaceParent)(IUnknown**)
	{
		return E_NOTIMPL;
	}

	/***************************************************************************
	* Function:	IDispatchExImpl::GetNextDispID()
	* Args:		<DWORD> dwGrfDex - the flags to fetch with
	*			<DISPID> dispid - the dispid the caller is currently on
	*			<DISPID*> pdispid - the out parameter for the next ID
	* Returns:	<HRESULT> Standard COM codes
	* Purpose:	Returns the next dispid in the list
	***************************************************************************/
	STDMETHOD(GetNextDispID)(DWORD dwGrfDex, DISPID dispid, DISPID* pDispID)
	{
		// Serialize access, since another thread might try to modify members at
		// the same time
	//	RefCountExclusiveLock lock(m_csAccessSync);

		dwGrfDex;
		// We need to check for the request of the first DISPID
		if (dispid != DISPID_STARTENUM)
		{
			// Get the next dispid
			return GetNextDispID(dispid,pDispID);
		}
		else
		{
			// Find the first dispid in the map
			return GetStartDispID(pDispID);
		}
	}

	/***************************************************************************
	* Function:	IDispatchExImpl::InvokeEx()
	* Args:		<DISPID> dispid - the dispid of the entry to invoke
	*			<LCID> lcid - the locale id
	*			<WORD> wFlags - calling flags
	*			<DISPPARAMS*> pDispParams - the dispparams of the method
	*			<VARIANT*> pvtResult - the variant holding the result
	*			<EXCEPINFO*> pExcepInfo - the exception info pointer
	*			<IServiceProvider*> ignored
	* Returns:	<HRESULT> Standard COM codes
	* Purpose:	Processes the callers request to execute a method. This can be
	*			a property get/put or a function call.
	***************************************************************************/
	STDMETHOD(InvokeEx)(DISPID dispid, LCID lcid, WORD wFlags,
		DISPPARAMS* pDispParams, VARIANT* pvtResult, EXCEPINFO* pExcepInfo,
		IServiceProvider*)
	{
		// Serialize access, since another thread might try to modify members at
		// the same time we are accessing them
	//	RefCountExclusiveLock lock(m_csAccessSync);

		HRESULT hr = E_FAIL;
		try
		{	
			// Check to see if we are retrieving a property
			if (wFlags & DISPATCH_PROPERTYGET)
			{
				// Validate that they passed in a valid variant for results
				// and have no parameters assigned to the "Get"
				if (pvtResult &&
					(pDispParams == 0 || pDispParams->cArgs == 0))
				{
					// Do the "Get" with the specified ID
					hr = GetVariantByDispID(dispid,pvtResult);
				}
				else
				{
					// They passed in bogus stuff
					hr = E_INVALIDARG;
				}
			}

			// Check to see if we are setting a property
			if (wFlags & (DISPATCH_PROPERTYPUT | DISPATCH_PROPERTYPUTREF))
			{
				// Validate that params passed in
				if (pDispParams && (pDispParams->cArgs == 1 ||
					(pDispParams->cNamedArgs == 1 &&
					pDispParams->rgdispidNamedArgs[0] == DISPID_PROPERTYPUT)))
				{
					// Set the new value to the variant in the first index
					hr = SetVariantByDispID(dispid,&pDispParams->rgvarg[0]);
				}
				else
				{
					// They parameter is not optional
					hr = DISP_E_PARAMNOTOPTIONAL;
				}
			}

			// Finally, check to see if we are invoking a function
			if (wFlags & DISPATCH_METHOD)
			{
				hr = InvokeByDispID(dispid, lcid, wFlags, pDispParams, pvtResult, pExcepInfo);
			}

			// Check to see if this dispid is in the typeinfo object
			if (hr == S_FALSE)
			{
				UINT nErrArg = 0;
				// Forward to the type info
				hr = m_pti->Invoke(this,dispid,wFlags,pDispParams,pvtResult,pExcepInfo,&nErrArg);
			}
		}
		catch (...)
		{
			hr = E_UNEXPECTED;
		}

		return hr;
	}
};


////////////////////////////////////////////////////////////////////////////////////
// Class that holds the dynamic dispatch info
class CDynamicDispatchEntry
{
private:
	// For determing the type and state of the entry
	bool	m_bUseTypeInfo;
	bool	m_bIsDeleted;

	VARIANT	m_vtVar;	// Holds the data at this entry
	BSTR	m_bstrName; // The name of the entry as the script engine sees it
	DISPID	m_dispid;	// The dispid of the entry as returned to the engine

public:
	// Default constructor
	CDynamicDispatchEntry() 
		: m_bUseTypeInfo(false)
		, m_bIsDeleted(true)
		, m_bstrName(0)
		, m_dispid(-1) 
	{
		VariantInit(&m_vtVar);
	}
	
	// An empty entry (same as deleted)
	CDynamicDispatchEntry(BSTR bstrName, DISPID dispid) 
		: m_bUseTypeInfo(false)
		, m_bIsDeleted(true)
		, m_dispid(dispid)
		, m_bstrName(SysAllocString(bstrName)) 
	{
		VariantInit(&m_vtVar);
	}

	// A completely specified entry
	CDynamicDispatchEntry(BSTR bstrName, DISPID dispid, VARIANT vtVar, bool bIsFunc = false)
		: m_bUseTypeInfo(bIsFunc)
		, m_bIsDeleted(false)
		, m_dispid(dispid)
		, m_bstrName(SysAllocString(bstrName))
	{
		VariantInit(&m_vtVar);
		VariantCopy(&m_vtVar, &vtVar);
	}

	// Default destructor
	virtual ~CDynamicDispatchEntry() 
	{
		if (m_bstrName)
		{
			SysFreeString(m_bstrName);
			m_bstrName = 0;
		}
		VariantClear(&m_vtVar);
	}

	// Attributes
	inline BSTR GetName(void) { return m_bstrName; }
	inline DISPID GetDispID(void) { return m_dispid; }
	inline bool IsDeleted(void) { return m_bIsDeleted; }
	inline bool UseTypeInfo(void) { return m_bUseTypeInfo; }
	inline HRESULT GetVar(VARIANT* pvtDest)
	{
		// Copy our variant into the dest
		if (m_vtVar.vt == VT_EMPTY)
		{
			pvtDest->vt = VT_EMPTY;
			return S_FALSE;
		}
		return VariantCopy(pvtDest, &m_vtVar);
	}
	inline HRESULT SetVar(VARIANT* pvtSrc)
	{
		// If we are setting this property, it must not be deleted
		m_bIsDeleted = false;

		// Clear out whatever was already there
		HRESULT hr = VariantClear(&m_vtVar);
		if (SUCCEEDED(hr))
		{
			// Copy their variant into our variant
			hr = VariantCopy(&m_vtVar, pvtSrc);
		}
		return hr;
	}

	// Operations
	inline HRESULT MarkDeleted(void)
	{
		HRESULT hr = S_FALSE;
		// Determine if this is a function provided by an ancestor dispatch class
		if (!m_bUseTypeInfo)
		{
			VariantClear(&m_vtVar);
			m_bIsDeleted = true;
			hr = S_OK;
		}
		return hr;
	}
};

#endif // __IDispatchExImpl_H__