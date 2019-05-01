/// <summary>Support for event-based message passing.</summary>
///
/// Classes for subscribing, unsubscribing, and raising events.
///
/// \file event.h

#pragma once

#include <glge/common.h>

#include <functional>
#include <unordered_map>

namespace glge::event
{
  /// <summary>
  /// A registry for handler functions to be notified of events along with a
  /// passed message.
  /// </summary>
  /// <typeparam name="EventArgs">
  /// Type of message object to pass to handler functions.
  /// </typeparam>
	template<typename EventArgs>
	class Event
	{
	public:
    /// <summary>Type for functions that handle this type of Event.</summary>
		using handler_type = std::function<void(EventArgs)>;

	private:
		// At least 4294967296 subscriptions can be created using this type
		using id_type = unsigned long;

		std::unordered_map<id_type, handler_type> listeners;

	public:
    /// <summary>
    /// A subscription to an Event. Can be used to
    /// unsubscribe from receiving notifications
    /// of future events.
    /// </summary>
		class Subscription
		{
		private:
			static id_type next_id;

			Event<EventArgs> & event;
			id_type id;

		public:
      /// <summary>
      /// Subscribes to the given event using the provided handler function.
      /// </summary>
      /// <param name="event">Event to subscribe to.</param>
      /// <param name="handler">Handler function for the event.</param>
			Subscription(Event<EventArgs> & event,
						 const handler_type & handler) :
				event(event),
				id(next_id++)
			{
				auto [iterator, success] =
					event.listeners.insert({id, handler});

				if (!success)
				{
					throw std::runtime_error(
						"There are too many event subscriptions active");
				}
			}

      /// <summary>Unsubscribe from future event notifications.</summary>
			void release() { event.listeners.erase(id); }
		};

    /// <summary>
    /// Signal the occurrence of an event, passing a copy of the given
    /// message to each subscriber.
    /// </summary>
    /// <param name="args">
    /// Message object to pass event info to subscribers.
    /// </param>
		void raise(const EventArgs & args)
		{
			auto raise_handler =
				[&](const std::pair<id_type, handler_type> & pair) {
					pair.second(args);
				};

			std::for_each(listeners.cbegin(), listeners.cend(), raise_handler);
		}

    /// <summary>
    /// Register a handler to receive notifications of future events. 
    /// Returned Subscription can be used to unsubscribe.
    /// </summary>
    /// <param name="handler">
    /// Handler function to be invoked when this Event is raised.
    /// </param>
    /// <returns>
    /// Subscription allowing future unsubscription from further notifications.
    /// </returns>
		Subscription register_handler(const handler_type & handler)
		{
			return Subscription(*this, handler);
		}
	};

	template<typename EventArgs>
	typename Event<EventArgs>::id_type Event<EventArgs>::Subscription::next_id =
		0;
}   // namespace glge::event
