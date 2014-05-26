// DDASEvent.h
//
// A class to aggregate separate ddaschannel objects.
//
// Author: Jeromy Tompkins
// Date  : 5/6/2013   
//

#ifndef DDASEVENT_H
#define DDASEVENT_H

#include <vector>
#include <TObject.h>
#include "ddaschannel.hh"

class DDASEvent : public TObject
{
    private:
        std::vector<ddaschannel*> m_data; // Extensible array of primitive  ddaschannel objects

    public:
        //! Default constructor
        DDASEvent();

        //! Copy constructor
        /*!
         *   Implements a deep copy
         */
        DDASEvent(const DDASEvent& obj);

        //! Assignment operator
        /*!
         * Performs a deep copy of the data belonging to obj.
         * There is no attempt to make this exception-safe.
         */
        DDASEvent& operator=(const DDASEvent& obj);

        //! Destructor
        /*!
         * Deletes the ddaschannel data objects! 
         */
        ~DDASEvent();

        //! Access internal, extensible array of channel data 
        /*!
         *   @return reference to the data 
         */ 
        std::vector<ddaschannel*>& GetData() { return m_data;}

        //! Append channel data to event
        /*!
         * Appends the pointer to the internal, extensible data array.
         * There is no check that the object pointed to by the argument
         * exists, so that it is the user's responsibility to implement.
         *
         *   @param channel pointer to a ddaschannel object
         */
        void AddChannelData(ddaschannel* channel);

        //! Get number of channel-wise data in event
        UInt_t GetNEvents() const { return m_data.size(); }

        //! Get timestamp of first channel datum
        /*!
         *   If data exists return timestamp of first element in the array. This should
         *   be the earliest unit of data stored by this object. If no data exists, 
         *   returns 0.
         */
        Double_t GetFirstTime() const;

        //! Get timestamp of last channel datum
        /*!
         *   If data exists return timestamp of last element in the array. This should
         *   be the most recent unit of data stored by this object. If no data exists, 
         *   returns 0.
         */
        Double_t GetLastTime() const;

        //! Get time difference between first and last channel data
        /*!
         *   If data exists return timestamp of first and last units of data stored
         *   by this object. If no data exists, return 0;
         */
        Double_t GetTimeWidth() const;

        //! Clear data vector and reset event
        /*!
         * Deletes the ddaschannel data objects and resets the 
         * size of the extensible data array to zero  
         */ 
        void Reset();

        ClassDef(DDASEvent,1);
};

#endif
