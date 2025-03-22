/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shinckel <shinckel@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/22 18:02:20 by shinckel          #+#    #+#             */
/*   Updated: 2025/03/22 19:45:28 by shinckel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../headers/Socket.hpp"

int main(int argc, char **argv) {
  if (argc != 3) {
    std::cerr << "Usage: " << argv[0] << " <port> <password>";
    return 1;
  }

  // start server
  Socket server(argv[1], argv[2]);

  return 0;  
}
