/***************************************************************************
  tag: Peter Soetens  Mon Jan 19 14:11:26 CET 2004  TypeInfo.hpp

                        TypeInfo.hpp -  description
                           -------------------
    begin                : Mon January 19 2004
    copyright            : (C) 2004 Peter Soetens
    email                : peter.soetens@mech.kuleuven.ac.be

 ***************************************************************************
 *   This library is free software; you can redistribute it and/or         *
 *   modify it under the terms of the GNU General Public                   *
 *   License as published by the Free Software Foundation;                 *
 *   version 2 of the License.                                             *
 *                                                                         *
 *   As a special exception, you may use this file as part of a free       *
 *   software library without restriction.  Specifically, if other files   *
 *   instantiate templates or use macros or inline functions from this     *
 *   file, or you compile this file and link it with other files to        *
 *   produce an executable, this file does not by itself cause the         *
 *   resulting executable to be covered by the GNU General Public          *
 *   License.  This exception does not however invalidate any other        *
 *   reasons why the executable file might be covered by the GNU General   *
 *   Public License.                                                       *
 *                                                                         *
 *   This library is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU     *
 *   Lesser General Public License for more details.                       *
 *                                                                         *
 *   You should have received a copy of the GNU General Public             *
 *   License along with this library; if not, write to the Free Software   *
 *   Foundation, Inc., 59 Temple Place,                                    *
 *   Suite 330, Boston, MA  02111-1307  USA                                *
 *                                                                         *
 ***************************************************************************/

#ifndef ORO_CORELIB_TYPEINFO_HPP
#define ORO_CORELIB_TYPEINFO_HPP

#include <vector>
#include <string>
#include "../base/DataSourceBase.hpp"
#include "../base/ChannelElementBase.hpp"

namespace RTT
{ namespace types {

    /**
     * A class for representing a user type, and which can build
     * instances of that type.
     */
    class RTT_API TypeInfo
    {
    protected:
        typedef std::vector<TypeConstructor*> Constructors;
        typedef std::vector<TypeTransporter*> Transporters;
        Constructors constructors;
        Transporters transporters;
    public:
        virtual ~TypeInfo();
        /**
         * Return unique the type name.
         */
        virtual const std::string& getTypeName() const = 0;

        /**
         * Installs the type info object in the global data source type info handler.
         * This will be called by the TypeInfoRepository, prior to registering this
         * type. If installation fails the TypeInfoRepository will delete this object,
         * and all its associated constructors.
         *
         * @retval true installation succeeded. This object should not be deleted
         * during the execution of the current process.
         * @retval false installation failed. This object is not used and may be deleted.
         */
        virtual bool installTypeInfoObject() = 0;

        /**
         * @name Type building/factory functions
         * Used to create objects that hold data of a certain type.
         * @{
         */
        /**
         * Build a non modifyable instance of this type.
         * @param sizehint For variable size instances, use it to hint
         * the size of the instance.
         */
        virtual base::AttributeBase* buildConstant(std::string name,base::DataSourceBase::shared_ptr, int sizehint) const;
        virtual base::AttributeBase* buildConstant(std::string name,base::DataSourceBase::shared_ptr) const = 0;
        /**
         * Build a modifyable instance of this type.
         * @param sizehint For variable size instances, use it to hint
         * the size of the instance.
         */
        virtual base::AttributeBase* buildVariable(std::string name,int sizehint) const;
        virtual base::AttributeBase* buildVariable(std::string name) const = 0;

        /**
         * Tries to resize a data source in case it's a resizable sequence.
         * @return true if the resizing could be done, false otherwise.
         */
        virtual bool resize(base::DataSourceBase::shared_ptr arg, int size) const;

        /**
         * Constructor syntax: construct a internal::DataSource which returns an instance of data
         * depending on the given arguments.  When \a args is empty, the default 'variable'
         * is returned.
         */
        virtual base::DataSourceBase::shared_ptr construct(const std::vector<base::DataSourceBase::shared_ptr>& args) const;

        /**
         * Automatic conversion: convert a internal::DataSource to this type.
         * For example, for converting float -> double. If no valid conversion was found,
         * returns arg again, otherwise, a new data source.
         */
        virtual base::DataSourceBase::shared_ptr convert(base::DataSourceBase::shared_ptr arg) const;

        /**
         * Add a constructor/convertor object.
         */
        virtual void addConstructor(TypeConstructor* tb);

        /**
         * build an alias with b as the value.  If b is of the wrong type,
         * 0 will be returned..
         */
        virtual base::AttributeBase* buildAlias(std::string name, base::DataSourceBase::shared_ptr b ) const = 0;

        /**
         * Build a Property of this type.
         */
        virtual base::PropertyBase* buildProperty(const std::string& name, const std::string& desc, base::DataSourceBase::shared_ptr source = 0) const = 0;

        /**
         * Build an Attribute of this type.
         */
        virtual base::AttributeBase* buildAttribute(std::string name, base::DataSourceBase::shared_ptr source = 0 ) const = 0;

        /**
         * Build a internal::ValueDataSource of this type.
         */
        virtual base::DataSourceBase::shared_ptr buildValue() const = 0;

        /** Build a internal::ReferenceDataSource of this type, pointing to the given
         * pointer
         */
        virtual base::DataSourceBase::shared_ptr buildReference(void* ptr) const = 0;

        /**
         * Returns a DataSource that first executes an action and returns the result of another data source.
         * If \a source is an AssignableDataSource, an AssignableDataSource is returned of the same type, otherwise,
         * a plain DataSource is returned.
         */
        virtual base::DataSourceBase::shared_ptr buildActionAlias(base::ActionInterface* action, base::DataSourceBase::shared_ptr source) const = 0;
        /** @} */

        /**
         * @name Conversion to/from text
         * Used to convert data to human readable text and vice versa.
         * @{
         */
        /**
         * Output this datasource as a human readable string.
         * The default just writes the type name in parentheses to \a os.
         */
        virtual std::ostream& write(std::ostream& os, base::DataSourceBase::shared_ptr in ) const = 0;

        /**
         * Read a new value for this datasource from a human readable string.
         * The default does nothing.
         */
        virtual std::istream& read(std::istream& os, base::DataSourceBase::shared_ptr out ) const = 0;

        /**
         * Usability function which converts data to a string.
         */
        virtual std::string toString( base::DataSourceBase::shared_ptr in ) const;

        /**
         * Usability function which converts a string to data.
         */
        virtual bool fromString( const std::string& value, base::DataSourceBase::shared_ptr out ) const;

        /**
         * Returns true if this type is directly streamable
         * using read()/write() or toString()/fromString().
         */
        virtual bool isStreamable() const = 0;
        /** @} */

        /**
         * @name Inspecting data structures.
         * Used to write a complex type to an external representation, like XML.
         * @{
         */

        /**
         * Returns the list of struct member names of this type.
         * In case this type is not a struct, returns an empty list.
         */
        virtual std::vector<std::string> getMemberNames() const;

        /**
         * Returns a member of a given data source struct identified by its name.
         * @param item The item of which to return a reference to a member
         * @param name The name of a member within \a item. Is a name of a member in case of a struct
         * or an index number in case of a sequence.
         * @return null if no such member exists, an assignable datasource referencing that member otherwise.
         */
        virtual base::DataSourceBase::shared_ptr getMember(base::DataSourceBase::shared_ptr item, const std::string& name) const;

        /**
         * Returns a member of a given data source struct identified by a data source id.
         * @param item The item of which to return a member
         * @param id   Or a string data source containing the name of a member if item is a struct,
         * Or an unsigned int data source containing the index of an element if item is a sequence
         */
        virtual base::DataSourceBase::shared_ptr getMember(base::DataSourceBase::shared_ptr item,
                                                           base::DataSourceBase::shared_ptr id) const;

        /**
         * Compose a type (target) from a DataSourceBase (source) containing its members.
         * The default behavior tries to assign \a source to \a target. If that fails,
         * it tries to decompose \a target into its members and update the members of \a target with the contents of source.
         *
         * The default implementation in TemplateTypeInfo works for most types, but can be overridden in case there are
         * multiple versions/possibilities to make a \a target from a \a source. For example, in
         * order to support legacy formats or in order to do the inverse of decomposeType().
         *
         * @param source A data source of the same type as \a target OR a PropertyBag that contains the parts of \a target
         * to be refreshed.
         * @param target A data source of the same type as this TypeInfo object which contains the data to be updated from \a source.
         * @return true if source could be updated, false otherwise.
         *
         * @see types::propertyDecomposition and types::typeDecomposition for the inverse function, decomposing a type into
         * datasources and hierarchical properties.
         * @see decomposeType to do the inverse operation.
         */
        virtual bool composeType( base::DataSourceBase::shared_ptr source, base::DataSourceBase::shared_ptr target) const = 0;

        /**
         * Specialize this function to return an alternate type which represents this one in a compatible way.
         * For example, a short converts to an long or an enum to an int or a string.
         * If your return a datasource containing a property bag, then this function should do the inverse of
         * composeType: the returned property bag contains all parts of the current type (\a source) which can be modified and merged back
         * into this type with composeType. Mathematically: composeType( decomposeType( A ), B); assert( A == B );
         * @return null in order to indicate that decomposition through getMember() may be tried. You may return \a source itself in order
         * to prevent any further decomposition of your type (using getMember(), which is used as fall-back by the rest
         * of the software. For example, to avoid that a string is decomposed
         * into a sequence of chars, or to avoid that a primitive type like 'int' is further queried.
         */
        virtual base::DataSourceBase::shared_ptr decomposeType(base::DataSourceBase::shared_ptr source) const;

        /**
         * Specialize this function to return an alternate type which represents this one in a compatible way.
         * For example, a short converts to an long or an enum to an int or a string.
         * @return null if this type is not convertible to anything else.
         * @deprecated by decomposeType. We want to rename convertType to decomposeType. This function is left
         * here for transitional purposes.
         */
        virtual base::DataSourceBase::shared_ptr convertType(base::DataSourceBase::shared_ptr source) const;

        /**
         * @}
         */

        /**
         * @name Distribution of objects
         * Used to transport data over a network.
         * @{
         */

        /**
         * Register a protocol for data transport over a network.
         */
        bool addProtocol(int protocol_id, TypeTransporter* tt);

        /**
         * Returns this type's transport for a given protocol.
         * Will print a warning when returning null. Use
         * hasProtocol() to query without warning.
         */
        TypeTransporter* getProtocol(int protocol_id) const;

        /**
         * Check if this type is transporable over a given protocol.
         */
        bool hasProtocol(int protocol_id) const;

        /**
         * Return a list of protocols that have transporters
         */
        std::vector<int> getTransportNames() const;

        typedef const std::type_info * TypeId;
        virtual TypeId getTypeId() const = 0;
        virtual const char * getTypeIdName() const = 0;

        /**
         * Returns a new InputPort<T> object where T is the type represented by
         * this TypeInfo object.
         */
        virtual base::InputPortInterface* inputPort(std::string const& name) const = 0;

        /**
         * Returns a new OutputPort<T> object where T is the type represented by
         * this TypeInfo object.
         */
        virtual base::OutputPortInterface* outputPort(std::string const& name) const = 0;

        /**
         * Creates single data or buffered storage for this type.
         *
         * @param policy Describes the kind of storage requested by the user
         * @return a storage element.
         */
        virtual base::ChannelElementBase::shared_ptr buildDataStorage(ConnPolicy const& policy) const = 0;
        virtual base::ChannelElementBase::shared_ptr buildChannelOutput(base::InputPortInterface& port) const = 0;
        virtual base::ChannelElementBase::shared_ptr buildChannelInput(base::OutputPortInterface& port) const = 0;

    protected:
        /**
         * Migrates all protocols present in \a orig to this
         * type info object. It is meant as a helper when a type
         * info object is replaced by a new instance.
         * @pre This type has no transports registered yet, ie,
         * it is newly constructed.
         */
        void migrateProtocols(TypeInfo* orig);
    };

}}

#endif
