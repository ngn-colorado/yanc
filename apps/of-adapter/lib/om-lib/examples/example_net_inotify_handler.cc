//
//  Olli's C++ Library
//  examples/example_net_inotify_handler.cc
//  (c) 2013 Oliver Michel <oliver dot michel at editum dot de>
//

#include <iostream>
#include <sys/inotify.h>
#include <sys/types.h>
#include <typeinfo>
#include <unistd.h>

#include <om/net/agent.h>
#include <om/net/net.h>
#include <om/net/inotify_handler.h>


class ExampleINotifyAgent : public om::net::Agent {

	void device_ready(timeval* timestamp, om::net::IOInterface* iface) {

		if(typeid(*iface) == typeid(om::net::INotifyHandler)) {

			om::net::INotifyHandler* inotify = 0;
			char buf[2*(sizeof(struct inotify_event)+16)] = {0};
			ssize_t read_len = 0, parsed_len = 0;
			
			inotify = dynamic_cast<om::net::INotifyHandler*>(iface);
			read_len = inotify->read_event(buf, sizeof(buf));

			while(parsed_len < read_len) {

				struct inotify_event *event = (struct inotify_event*) &buf[parsed_len];

				if(event->len) {

					std::cout << inotify->pathname_for_watch(event->wd) << " (" 
						<< event->wd << "): " << event->name << " (" << event->mask 
						<< ")" << std::endl;
				}

				parsed_len += (sizeof(struct inotify_event) + event->len);
			}

		}
	}
};

int main(int argc, char const *argv[]) {
	
	// allocate previously defined agent
	ExampleINotifyAgent* a = new ExampleINotifyAgent;
	
	om::net::INotifyHandler* h = new om::net::INotifyHandler;
	h->add_watch("/home/olli/src/om-lib/", IN_CREATE | IN_DELETE);

	a->add_interface(h);

	// fire up the agent (blocking)
	a->run();

	return 0;
}
